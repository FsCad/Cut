Option Explicit On
Option Strict Off
Imports System.Math
Imports Autodesk.AutoCAD.DatabaseServices
Imports Autodesk.Civil.Corridor
Imports Autodesk.Civil.Alignment
Imports Surface = Autodesk.Civil.Surface.Surface
Imports Autodesk.Civil.ApplicationServices
Imports Autodesk.Civil
Imports Autodesk.Civil.Settings

Imports DBTransactionManager = Autodesk.AutoCAD.DatabaseServices.TransactionManager
Imports Autodesk.AutoCAD.EditorInput

Public Class Utilities


    ' version consts
    Public Const R2008 As String = "R2008"
    Public Const R2007 As String = "R2007"
    Public Const R2006 As String = "R2006"
    Public Const R2005 As String = "R2005"

    Public Const Side As String = "Side"
    Public Const Left As Integer = 1
    Public Const Right As Integer = 0
    Public Const ITrue As Integer = 1
    Public Const IFalse As Integer = 0

    Public Const PI As Double = 3.14159265

    Public Enum FillOrCut
        FillSituation
        CutSituation
        UndefinedSituation
    End Enum
    Public Enum ShoulderSubbaseType
        Shoulder
        Subbase
    End Enum

    Public Enum RoundingOption
        NoneType
        CircularType
        ParabolicType
    End Enum

    ' *************************************************************************
    ' *************************************************************************
    ' *************************************************************************
    '          Name: RecordError
    '
    '   Description: Utility function to record ErrorHandler: errors to event viewer
    '
    Public Shared Sub RecordError(ByVal corridorState As CorridorState, ByVal e As System.Exception)

        If corridorState Is Nothing Then
            Throw New ArgumentNullException("corridorState")
        End If
        If e Is Nothing Then
            Throw New ArgumentNullException("e")
        End If
        corridorState.RecordError(CorridorError.Failure, CorridorErrorLevel.Severe, e.Message, e.Source, True)

    End Sub

    ' *************************************************************************
    ' *************************************************************************
    ' *************************************************************************
    '          Name: RecordWarning
    '
    '   Description: Utility function to record ErrorHandler: errors to event viewer
    '
    Public Shared Sub RecordWarning(ByVal corridorState As CorridorState, ByVal corridorError As CorridorError, ByVal message As String, ByVal source As String)
        If corridorState Is Nothing Then
            Throw New ArgumentNullException("corridorState")
        End If
        corridorState.RecordError(corridorError, CorridorErrorLevel.AsWarning, message, source, True)
    End Sub
    ' *************************************************************************
    ' *************************************************************************
    ' *************************************************************************
    '          Name: RecordWarning
    '
    '   Description: Utility function to record ErrorHandler: errors to event viewer
    '
    Public Shared Sub RecordError(ByVal corridorState As CorridorState, ByVal corridorError As CorridorError, ByVal message As String, ByVal source As String)
        If corridorState Is Nothing Then
            Throw New ArgumentNullException("corridorState")
        End If
        corridorState.RecordError(corridorError, CorridorErrorLevel.Severe, message, source, True)
    End Sub

    ' *************************************************************************
    ' *************************************************************************
    ' *************************************************************************
    '       Name: AdjustOffset
    '
    ' Desription:
    '    if SA works on an fixed offset baseline, the input offset need to be adjusted to base on the fixed position
    '    if SA works on the main base line, the input offset don't need to be adjusted, it's based on the main baseline alignment
    '    if SA works on transition baseline, the input offset don't need to be adjusted, it's based on the transition baseline alignment
    '
    ' Input: oRwyState
    '   current corridor state of the subassembly.
    '
    ' Input:    dOffset
    '   the value of user input offset
    '
    ' return:   the value of actual offset value
    '
    Public Shared Function AdjustOffset(ByVal oRwyState As CorridorState, ByVal dOffset As Double) As Double
        If oRwyState Is Nothing Then
            Throw New ArgumentNullException("oRwyState")
        End If

        If oRwyState.Mode = CorridorMode.Layout Then
            AdjustOffset = dOffset
        Else
            If oRwyState.CurrentAlignmentIsOffsetAlignment And oRwyState.CurrentAssemblyOffsetIsFixed Then
                AdjustOffset = dOffset + oRwyState.CurrentAssemblyFixedOffset
            Else
                AdjustOffset = dOffset
            End If
        End If

    End Function


    Public Shared Function GetVersion(ByVal rwState As CorridorState) As String

        If rwState Is Nothing Then
            Throw New ArgumentNullException("rwState")
        End If

        Dim oParamsString As ParamStringCollection

        oParamsString = rwState.ParamsString

        Dim vVersion As String
        Try
            vVersion = oParamsString.Value("Version")
        Catch
            vVersion = "R2005" 'if no version in ATC file, assume it is R2005
        End Try

        GetVersion = vVersion
    End Function

    Public Enum RoundingBy
        ByLength
        ByRadius
    End Enum
    ' *************************************************************************
    ' *************************************************************************
    ' *************************************************************************
    '       Name: GetAlignmentAndOrigin
    '
    ' Desription:
    '    considering different kinds of alignment type, ( main baseline, fixed offset baseline, transition offset baseline)
    '    we will get different working and origin point
    '
    ' Input: rwState
    '   current corridor state of the subassembly.
    '
    ' Output: alignment
    '   return correct alignment of the subassembly.
    '   alignment is the alignment of main baseline or offset baseline, according to the type of offset baseline.
    '
    ' Output: oOrigin
    '   return adjusted origin point of the subassembly.
    '
    Public Shared Sub GetAlignmentAndOrigin(ByVal rwState As CorridorState, ByRef oAlignmentId As ObjectId, ByRef oOrigin As PointInMem)

        If rwState Is Nothing Then
            Throw New ArgumentNullException("rwState")
        End If

        ' create new point if output parameter is null
        If oOrigin Is Nothing Then oOrigin = New PointInMem

        ' if in layout mode, then exit sub
        If rwState.Mode = CorridorMode.Layout Then
            oOrigin.Station = 0.0#
            oOrigin.Offset = 0.0#
            oOrigin.Elevation = 0.0#
            Exit Sub
        End If

        ' in corridor mode
        Dim isFixedAlignmentOffset As Boolean
        isFixedAlignmentOffset = (rwState.CurrentAlignmentIsOffsetAlignment And _
                                   rwState.CurrentAssemblyOffsetIsFixed)

        oOrigin.Station = rwState.CurrentStation

        If isFixedAlignmentOffset Then
            oAlignmentId = rwState.CurrentBaselineId
            oOrigin.Offset = rwState.CurrentOffset + rwState.CurrentAssemblyFixedOffset
            oOrigin.Elevation = rwState.CurrentElevation + rwState.CurrentAssemblyFixedElevation
        Else
            oAlignmentId = rwState.CurrentAlignmentId
            oOrigin.Offset = rwState.CurrentOffset
            oOrigin.Elevation = rwState.CurrentElevation
        End If
    End Sub
    ' *************************************************************************
    ' *************************************************************************
    ' *************************************************************************
    '          Name: CalcElevationOnSurface
    '
    '   Description: Calculate a point elevation, the point is on the surface.
    '                This point is represented by an alignment, station, offset.
    '
    '        oSurface:              INPUT the surface
    '        alignment:            INPUT the alignment
    '        dStationOnAlignment:   INPUT station on the alignment
    '        dOffsetToAlignment:    INPUT offset to the alignment
    '        dElevationOnSurface:   OUTPUT the elevation
    '
    Public Shared Sub CalcElevationOnSurface( _
    ByVal surfaceId As ObjectId, _
    ByVal alignmentId As ObjectId, _
    ByVal stationOnAlignment As Double, _
    ByVal offsetToAlignment As Double, _
    ByRef elevationOnSurface As Double)

        Dim surface As Surface
        Dim alignment As Alignment
        Dim x As Double, y As Double
        Dim db As Database = HostApplicationServices.WorkingDatabase
        Dim tm As DBTransactionManager = db.TransactionManager
        surface = tm.GetObject(surfaceId, OpenMode.ForRead, False, False)
        alignment = tm.GetObject(alignmentId, OpenMode.ForRead, False, False)

        alignment.PointLocation(stationOnAlignment, offsetToAlignment, x, y)
        elevationOnSurface = surface.FindElevationAtXY(x, y)
    End Sub
    ' *************************************************************************
    ' *************************************************************************
    ' *************************************************************************
    '       Name: GetRoundingCurve
    '
    ' Desription:
    '    Get Rounding Curve's tesselated points.
    '
    ' Input: oIntersectPt ----- the Intersection point of two slope
    '        dSlope1 ---------- the first slope( to oIntersectPt)
    '        dSlope2 ---------- the second slope( from oIntersectPt)
    '        nRoundingOption--- CurveType or ParabolicType
    '        nRoundingBy ------ ByLength or ByRadius
    '        dRoundingValue---- Length value or Radius value
    '        nRoundingTesselation --- number of intermittant points on rounding links
    '        dTangentLength1 ------   if the pre tangent length of the curve is over it, just use this length.
    '        dTangentLength2 ------   if the post tangent length of the curve is over it, just use this length.
    '        isRight         ------   specifies right or left side.
    ' output: tesselatedPts --------- the tesselated Points on the Curve
    '
    Public Shared Function GetRoundingCurve(ByVal oIntersectPt As IPoint, ByVal dSlope1 As Double, ByVal dSlope2 As Double, _
                                             ByVal nRoundingOption As RoundingOption, ByVal nRoundingBy As RoundingBy, ByVal dRoundingValue As Double, _
                                            ByVal nRoundingTesselation As Long, ByVal dTangentLength1 As Double, ByVal dTangentLength2 As Double, _
                                            ByVal isRight As Boolean, ByRef tesselatedPts() As IPoint) As Boolean
        On Error Resume Next
        GetRoundingCurve = False

        If nRoundingOption = RoundingOption.NoneType Then
            Exit Function
        ElseIf nRoundingOption = RoundingOption.CircularType Then
            Dim dAngle1 As Double 'pre tangent's angle
            Dim dAngle2 As Double 'post tangent's angle
            dAngle1 = Atan(dSlope1)
            If dAngle1 < 0 Then
                If isRight Then
                    dAngle1 = 2 * PI + dAngle1
                Else
                    dAngle1 = PI - dAngle1
                End If
            ElseIf dAngle1 > 0 Then
                If isRight Then
                    dAngle1 = dAngle1
                Else
                    dAngle1 = PI - dAngle1
                End If
            Else
                If isRight Then
                    dAngle1 = 0.0#
                Else
                    dAngle1 = PI
                End If
            End If
            dAngle1 = dAngle1 + PI
            If dAngle1 > 2 * PI Then dAngle1 = dAngle1 - 2 * PI

            dAngle2 = Atan(dSlope2)
            If dAngle2 < 0 Then
                If isRight Then
                    dAngle2 = 2 * PI + dAngle2
                Else
                    dAngle2 = PI - dAngle2
                End If
            ElseIf dAngle2 > 0 Then
                If isRight Then
                    dAngle2 = dAngle2
                Else
                    dAngle2 = PI - dAngle2
                End If
            Else
                If isRight Then
                    dAngle2 = 0.0#
                Else
                    dAngle2 = PI
                End If
            End If

            Dim dArcAngle As Double
            dArcAngle = Abs(PI - Abs(dAngle1 - dAngle2))

            Dim dRadius As Double
            dRadius = dRoundingValue
            If nRoundingBy = RoundingBy.ByLength Then dRadius = dRoundingValue / dArcAngle

            Dim dTangentLength As Double
            dTangentLength = dRadius * Tan(dArcAngle / 2.0#)
            If dTangentLength1 < dTangentLength Then dTangentLength = dTangentLength1
            If dTangentLength2 < dTangentLength Then dTangentLength = dTangentLength2
            dRadius = dTangentLength / Tan(dArcAngle / 2.0#)
            Dim vIntersectPt(0 To 2) As Double
            vIntersectPt(0) = oIntersectPt.Offset
            vIntersectPt(1) = oIntersectPt.Elevation
            vIntersectPt(2) = 0.0#
            Dim vStartPt() As Double

            vStartPt = Utility.PolarPoint(vIntersectPt, dAngle1, dTangentLength)
            Dim vEndPt() As Double
            vEndPt = Utility.PolarPoint(vIntersectPt, dAngle2, dTangentLength)

            Dim dIntPtAngle As Double 'the oIntersectPt's angle
            If Abs(dAngle2 - dAngle1) > PI Then
                dIntPtAngle = dAngle1 + (dAngle2 - dAngle1) / 2.0# + PI
            Else
                dIntPtAngle = dAngle1 + (dAngle2 - dAngle1) / 2.0#
            End If
            If dIntPtAngle > 2 * PI Then dIntPtAngle = dIntPtAngle - 2 * PI

            Dim vCenterPt(0 To 2) As Double
            vCenterPt(0) = oIntersectPt.Offset + dRadius / Cos(dArcAngle / 2.0#) * Cos(dIntPtAngle)
            vCenterPt(1) = oIntersectPt.Elevation + dRadius / Cos(dArcAngle / 2.0#) * Sin(dIntPtAngle)
            vCenterPt(2) = 0.0#

            Dim dStartAng As Double
            If Abs(vStartPt(0) - vCenterPt(0)) < 0.00001 Then
                If vStartPt(1) < vCenterPt(1) Then
                    dStartAng = PI * 3.0# / 2.0#
                Else
                    dStartAng = PI / 2.0#
                End If
            Else
                dStartAng = Atan((vStartPt(1) - vCenterPt(1)) / (vStartPt(0) - vCenterPt(0)))
                If dStartAng > 0 Then
                    If vStartPt(0) < vCenterPt(0) Then
                        dStartAng = PI + dStartAng
                    End If
                Else
                    If vStartPt(0) < vCenterPt(0) Then
                        dStartAng = PI + dStartAng
                    Else
                        dStartAng = PI * 2.0# + dStartAng
                    End If
                End If
            End If

            Dim dEndAng As Double
            If Abs(vEndPt(0) - vCenterPt(0)) < 0.00001 Then
                If vEndPt(1) < vCenterPt(1) Then
                    dEndAng = PI * 3.0# / 2.0#
                Else
                    dEndAng = PI / 2.0#
                End If
            Else
                dEndAng = Atan((vEndPt(1) - vCenterPt(1)) / (vEndPt(0) - vCenterPt(0)))
                If dEndAng > 0 Then
                    If vEndPt(0) < vCenterPt(0) Then
                        dEndAng = PI + dEndAng
                    End If
                Else
                    If vEndPt(0) < vCenterPt(0) Then
                        dEndAng = Pi + dEndAng
                    Else
                        dEndAng = Pi * 2.0# + dEndAng
                    End If
                End If
            End If

            Dim dDeltaAng As Double
            dDeltaAng = (dEndAng - dStartAng) / (nRoundingTesselation + 1)
            Dim i As Integer
            For i = 0 To nRoundingTesselation + 1
                Dim vCurrentPt() As Double
                vCurrentPt = Utility.PolarPoint(vCenterPt, dStartAng + i * dDeltaAng, dRadius)

                tesselatedPts(i) = New PointInMem
                tesselatedPts(i).Station = oIntersectPt.Station
                tesselatedPts(i).Offset = vCurrentPt(0)
                tesselatedPts(i).Elevation = vCurrentPt(1)
            Next i

        Else 'ParabolicType
            If Not isRight Then
                Dim dTempSwap As Double
                dTempSwap = dSlope2
                dSlope2 = -dSlope1
                dSlope1 = -dTempSwap

                dTempSwap = dTangentLength2
                dTangentLength2 = dTangentLength1
                dTangentLength1 = dTempSwap
            End If

            Dim dK As Double
            Dim dL As Double
            If nRoundingBy = RoundingBy.ByLength Then
                dL = dRoundingValue
                dK = (dSlope2 - dSlope1) / dL
            Else
                dK = dRoundingValue / 100.0#
                dL = (dSlope2 - dSlope1) / dK
                If dL < 0 Then
                    dK = -dK
                    dL = -dL
                End If
            End If

            Dim dT1 As Double
            dT1 = Abs(dL / 2.0# / Cos(Atan(dSlope1)))
            If dTangentLength1 < dT1 Then
                dT1 = dTangentLength1
                dL = dT1 * Cos(Atan(dSlope1)) * 2
            End If
            Dim dT2 As Double
            dT2 = Abs(dL / 2.0# / Cos(Atan(dSlope2)))
            If dTangentLength2 < dT2 Then
                dT2 = dTangentLength2
                dL = dT2 * Cos(Atan(dSlope2)) * 2
                dT1 = Abs(dL / 2.0# / Cos(Atan(dSlope1)))
            End If

            Dim dDeltaX As Double
            Dim i As Integer
            dDeltaX = dL / (nRoundingTesselation + 1)

            For i = 0 To nRoundingTesselation + 1
                Dim x As Double
                x = i * dDeltaX

                Dim nCurrentIndex As Integer
                If isRight Then
                    nCurrentIndex = i
                Else
                    nCurrentIndex = nRoundingTesselation + 1 - i
                End If

                tesselatedPts(nCurrentIndex) = New PointInMem
                tesselatedPts(nCurrentIndex).Station = oIntersectPt.Station
                tesselatedPts(nCurrentIndex).Offset = oIntersectPt.Offset + (x - dL / 2.0#)
                If x < dL / 2.0# Then
                    tesselatedPts(nCurrentIndex).Elevation = oIntersectPt.Elevation - (dL / 2.0# - x) * dSlope1 + dK / 2.0# * x * x
                Else
                    tesselatedPts(nCurrentIndex).Elevation = oIntersectPt.Elevation + (x - dL / 2.0#) * dSlope2 + dK / 2.0# * (dL * dL + x * x) - dL * dK * x
                End If
            Next i

        End If
        GetRoundingCurve = True

    End Function

    ' *************************************************************************
    ' *************************************************************************
    ' *************************************************************************
    '          Name: GetSide
    '
    '   Description: Convert "Side"  from the integer to corresponding enumeration value in the API.
    '
    '       vSide:      INPUT the integer "Side"
    '       
    '   returns: Corresponding enumeration value in the API.
    Public Shared Function GetSide(ByVal vSide As Integer) As Autodesk.Civil.Alignment.Side

        If vSide = Utilities.Left Then
            Return Autodesk.Civil.Alignment.Side.Left
        ElseIf vSide = Utilities.Right Then
            Return Autodesk.Civil.Alignment.Side.Right
        Else
            Return Autodesk.Civil.Alignment.Side.Both
        End If

    End Function

    ' *************************************************************************
    ' *************************************************************************
    ' *************************************************************************
    '          Name: CalcAlignmentOffsetToThisAlignment
    '
    '   Description: Calculate the offset from this alignment to other alignment
    '                Offset can be positive(right) or negtive(left)
    '
    '        thisAlignment:            INPUT this alignment
    '        stationOnThisAlignment:   INPUT station on this alignment
    '        otherAlignment:           INPUT the other alignment
    '        offsetToThisAlignment:    OUTPUT the offset from this alignment to the other alignment
    '        stationOnOtherAlignment:  OUTPUT the station on the other alignment
    '
    '   returns: true if successed, false if failed.
    '
    Public Shared Function CalcAlignmentOffsetToThisAlignment( _
        ByVal thisAlignmentId As ObjectId, _
        ByVal stationOnThisAlignment As Double, _
        ByVal otherAlignmentId As ObjectId, _
        ByRef offsetToThisAlignment As Double, _
        ByRef stationOnOtherAlignment As Double) As Boolean

        Return CalcAlignmentOffsetToThisAlignment(thisAlignmentId, _
                                                  stationOnThisAlignment, _
                                                  otherAlignmentId, _
                                                  Autodesk.Civil.Alignment.Side.Both, _
                                                  offsetToThisAlignment, _
                                                  stationOnOtherAlignment)

    End Function

    Public Shared Function CalcAlignmentOffsetToThisAlignment( _
        ByVal thisAlignmentId As ObjectId, _
        ByVal stationOnThisAlignment As Double, _
        ByVal otherAlignmentId As ObjectId, _
        ByVal enumSide As Side, _
        ByRef offsetToThisAlignment As Double, _
        ByRef stationOnOtherAlignment As Double) As Boolean

        Dim db As Database = HostApplicationServices.WorkingDatabase
        Dim tm As DBTransactionManager = db.TransactionManager
        Dim thisAlignment As Alignment
        Dim otherAlignment As Alignment
        Dim distance As Double
        Dim retVal As Boolean

        Try
            thisAlignment = tm.GetObject(thisAlignmentId, OpenMode.ForRead, False, False)
            otherAlignment = tm.GetObject(otherAlignmentId, OpenMode.ForRead, False, False)

            thisAlignment.DistanceToAlignment(stationOnThisAlignment, otherAlignment, enumSide, distance, stationOnOtherAlignment)

            Dim x As Double, y As Double
            otherAlignment.PointLocation(stationOnOtherAlignment, 0, x, y)

            'Dim station As Double
            'thisAlignment.StationOffset(x, y, station, offsetToThisAlignment)

            'calculate two more points...  pt2 is at "this Alignment" at the current station...pt3 is +/- a small station increment...
            Dim dX2 As Double, dY2 As Double
            Dim dX3 As Double, dY3 As Double
            Dim IncrementedStation As Double
            Dim StationIncrement As Double

            StationIncrement = 0.1

            IncrementedStation = stationOnThisAlignment + StationIncrement
            If IncrementedStation > thisAlignment.EndingStation Then
                StationIncrement = -0.1
                IncrementedStation = stationOnThisAlignment + StationIncrement
            End If

            thisAlignment.PointLocation(stationOnThisAlignment, 0, dX2, dY2)
            thisAlignment.PointLocation(IncrementedStation, 0, dX3, dY3)

            'determine whether point lies to left or right of "ThisAlignment"
            Dim Side As Double

            Dim Xa As Double, Ya As Double, Xb As Double, Yb As Double, Zcp As Double

            'determine cross product of vector a and b
            Xa = dX3 - dX2
            Ya = dY3 - dY2
            Xb = x - dX2
            Yb = y - dY2

            Zcp = Xa * Yb - Ya * Xb

            'if Zcp is (+) then pt lies on left of the alignment...if (-) then it lies on the right.  for left, side should be set to -1...
            If Zcp > 0 Then Side = -1 Else Side = 1
            'correct 'side' for alignment vector direction...
            If StationIncrement < 0 Then Side = -Side
            offsetToThisAlignment = distance * Side

            retVal = True
        Catch
            retVal = False
        End Try
        Return retVal

    End Function



    Public Shared Sub AddCodeToLink(ByVal i As Integer, ByVal iLinks As LinkCollection, ByVal linkIndex As Long, ByVal strArrCode(,) As String)
        If iLinks Is Nothing Then
            Throw New ArgumentNullException("iLinks")
        End If

        If i < 0 Or i > UBound(strArrCode, 1) Then
            Throw New ArgumentOutOfRangeException("i")
        End If

        Dim j As Integer
        'Attention - I'm using the error raising to get out of loop
        'When the second dimension will be reached, next loop will keck out - GV


        j = 0
        Do
            If j <= UBound(strArrCode, 2) Then
                If strArrCode(i, j) <> "" Then
                    iLinks(linkIndex).Codes.Add(strArrCode(i, j))
                Else
                    Exit Do
                End If
            Else
                Exit Do
            End If
            j = j + 1
        Loop

    End Sub

    Public Shared Sub AddCodeToPoint(ByVal i As Integer, ByVal iPoints As PointCollection, ByVal pointIndex As Long, ByVal strArrCode(,) As String)
        If iPoints Is Nothing Then
            Throw New ArgumentNullException("iPoints")
        End If

        If i < 0 Or i > UBound(strArrCode, 1) Then
            Throw New ArgumentOutOfRangeException("i")
        End If

        Dim j As Integer
        j = 0
        Do
            If j <= UBound(strArrCode, 2) Then
                If strArrCode(i, j) <> "" Then
                    iPoints(pointIndex).Codes.Add(strArrCode(i, j))
                Else
                    Exit Do
                End If
            Else
                Exit Do
            End If
            j = j + 1
        Loop

    End Sub

    ' *************************************************************************
    ' *************************************************************************
    ' *************************************************************************
    '          Name: IsProjectUnitsFeet
    '
    '   Description: Utility function to return the units
    Public Shared Function IsProjectUnitsFeet() As Boolean
        Dim settings As SettingsRoot
        settings = CorridorApplication.Application.ActiveDocument.Settings

        Dim drawingSettings As SettingsDrawing
        drawingSettings = settings.DrawingSettings

        Dim unitZone As SettingsUnitZone
        unitZone = drawingSettings.UnitZoneSettings

        If unitZone.DrawingUnits = DrawingUnitType.Feet Then
            IsProjectUnitsFeet = True   ' 1 foot = 12 inch
        Else
            IsProjectUnitsFeet = False ' 1 meter = 1000 mm
        End If
    End Function

    ' *************************************************************************
    ' *************************************************************************
    ' *************************************************************************
    '          Name: GetProjectUnitsDivisor
    '
    '   Description: Utility function to return the unit divisor.
    Public Shared Function GetProjectUnitsDivisor() As Double
        If IsProjectUnitsFeet() Then
            GetProjectUnitsDivisor = 12   ' 1 foot = 12 inch
        Else
            GetProjectUnitsDivisor = 1000 ' 1 meter = 1000 mm
        End If
    End Function

    ' always retuns % slope, for example, 0.02 (2%)
    Public Shared Function GetSlope(ByVal strShoulderSlope As String, ByVal rwState As CorridorState, ByVal shoulderSubbaseType As ShoulderSubbaseType, ByVal blnInsertLeft As Boolean) As Double
        If rwState Is Nothing Then
            Throw New ArgumentNullException("rwState")
        End If

        If IsNumeric(strShoulderSlope) Then
            GetSlope = Val(strShoulderSlope) / 100
            Exit Function
        End If

        '-----------------------------------------------------------------
        ' Get the true alignment
        'If offset baseline is unfixed, GetSlope returns the offset alignment's superelevation
        'else, returns the main baseline's sperelevation.

        Dim isFixedAlignmentOffset As Boolean
        isFixedAlignmentOffset = (rwState.CurrentAlignmentIsOffsetAlignment And _
                                   rwState.CurrentAssemblyOffsetIsFixed)

        Dim alignmentId As ObjectId
        Dim alignment As Alignment
        If isFixedAlignmentOffset Then
            alignmentId = rwState.CurrentBaselineId ' offset baseline has no CurrentAlignment
        Else
            alignmentId = rwState.CurrentAlignmentId
        End If
        alignment = HostApplicationServices.WorkingDatabase.TransactionManager().GetObject(alignmentId, OpenMode.ForRead, False)

        Dim sTempSlope As String
        sTempSlope = UCase(strShoulderSlope)

        'right now there is no diff between Shoulder and Subbase, but for the future changes, I'll separate them
        Try
            If shoulderSubbaseType = Utilities.ShoulderSubbaseType.Shoulder Then
                If blnInsertLeft Then
                    Select Case sTempSlope
                        Case "SI"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperLeftInShoulderCrossSlope)
                        Case "SO"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperLeftOutShoulderCrossSlope)
                        Case "LI"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperLeftInLaneCrossSlope)
                        Case "LO"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperLeftOutLaneCrossSlope)
                    End Select
                Else
                    Select Case sTempSlope
                        Case "SI"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperRightInShoulderCrossSlope)
                        Case "SO"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperRightOutShoulderCrossSlope)
                        Case "LI"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperRightInLaneCrossSlope)
                        Case "LO"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperRightOutLaneCrossSlope)
                    End Select
                End If
            ElseIf shoulderSubbaseType = Utilities.ShoulderSubbaseType.Subbase Then
                If blnInsertLeft Then
                    Select Case sTempSlope
                        Case "SI"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperLeftInShoulderCrossSlope)
                        Case "SO"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperLeftOutShoulderCrossSlope)
                        Case "LI"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperLeftInLaneCrossSlope)
                        Case "LO"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperLeftOutLaneCrossSlope)
                    End Select
                Else
                    Select Case sTempSlope
                        Case "SI"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperRightInShoulderCrossSlope)
                        Case "SO"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperRightOutShoulderCrossSlope)
                        Case "LI"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperRightInLaneCrossSlope)
                        Case "LO"
                            GetSlope = alignment.SuperElevationAtStation(rwState.CurrentStation).GetSegmentCrossSlope(SuperElevationCrossSegmentType.SuperRightOutLaneCrossSlope)
                    End Select
                End If
            End If
        Catch
        End Try
    End Function


    Public Shared Sub AddPoints( _
    ByVal size As Integer, _
    ByVal points As PointCollection, _
    ByVal pointArray() As Point, _
    ByVal dXArray() As Double, _
    ByVal dYArray() As Double, _
    ByVal sCodeArray() As String)

        If pointArray Is Nothing Then
            Throw New ArgumentNullException("pointArray")
        End If
        If points Is Nothing Then
            Throw New ArgumentNullException("points")
        End If

        Dim i As Integer
        For i = 1 To size
            pointArray(i) = points.Add(dXArray(i), dYArray(i), sCodeArray(i))
        Next

    End Sub

    Public Shared Function GetMarkedPoint(ByVal strMarkedPointName As String, ByVal corridorState As CorridorState) As ParamPoint

        If corridorState Is Nothing Then
            Throw New ArgumentNullException("corridorState")
        End If

        Dim paramsPoint As ParamPointCollection
        paramsPoint = corridorState.ParamsPoint
        GetMarkedPoint = Nothing
        If Not paramsPoint Is Nothing Then
            GetMarkedPoint = paramsPoint.Item(UCase(strMarkedPointName))
        End If

    End Function
End Class

