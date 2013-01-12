Option Explicit On
Option Strict Off
Imports System.Math
Imports Autodesk.AutoCAD.DatabaseServices
Imports Autodesk.Civil.Corridor
Imports Autodesk.Civil
Imports Autodesk.Civil.Alignment
Imports Autodesk.Civil.Profile
Imports Shape = Autodesk.Civil.Corridor.Shape
Imports DBTransactionManager = Autodesk.AutoCAD.DatabaseServices.TransactionManager


' **************************************************************************************************
'          ����: BasicLaneTransition
'
'          ����: Creates a simple cross-sectional representation of a corridor
'                lane composed of a single closed shape.  Attachment origin
'                is at top, most inside portion of lane.  The lane can
'                transition its width and cross-slope based on the position
'                supplied by an optional horizontal and vertical alignment.
'
' �߼�����: ����        ����     �Ƿ��ѡ   ȱʡֵ         ����
'                ---------------------------------------------------------------------------------------------------
'                TargetHA    ·��          yes                  none             ���ڹ��ȵ�ˮƽ�����·��
'                TargetVA    �ݶ���        yes                  none             ���ڹ��ȵĴ�ֱ������ݶ���
'
'    ����: ����          ����     �Ƿ��ѡ    ȱʡֵ          ����
'                ----------------------------------------------------------------------------------------------------
'                Side                    long                 yes                  Right               ָ��Ҫ��������������һ��
'                Width                  double             yes                  12.0                �������
'                Depth                  double             yes                  0.667              �ӽ����浽�׻������
'                Slope                  double            yes                  -0.02                �����İٷֱ��¶�
'                TransitionType   long                yes                  2                       ������·�ߺ�/���ݶ�����������ʱ�߼�ָ��ʱ����������Ϊ��ʽ
' **************************************************************************************************

Public Class MyBasicLaneTransition
    Inherits SATemplate


    Private Enum TransitionTypes ' Transition types supported
        kHoldOffsetAndElevation = 0
        kHoldElevationChangeOffset = 1
        kHoldGradeChangeOffset = 2
        kHoldOffsetChangeElevation = 3
        kChangeOffsetAndElevation = 4
    End Enum

    ' --------------------------------------------------------------------------
    ' Default values for input parameters
    Private Const SideDefault = Utilities.Right
    Private Const LaneWidthDefault = 12.0#
    Private Const LaneDepthDefault = 0.667
    Private Const LaneSlopeDefault = -0.02    '0.25 inch per foot
    Private Const HoldOriginalPositionDefault = TransitionTypes.kHoldOffsetAndElevation

    '��SATemplate���������Զ��岿����


    Protected Overrides Sub GetLogicalNamesImplement(ByVal corridorState As CorridorState)
        MyBase.GetLogicalNamesImplement(corridorState)


        ' Retrieve parameter buckets from the corridor state
        Dim oParamsLong As ParamLongCollection
        oParamsLong = corridorState.ParamsLong

        ' Add the logical names we use in this script
        Dim oParamLong As ParamLong
        oParamLong = oParamsLong.Add("TargetHA", ParamLogicalNameType.Alignment)
        oParamLong.DisplayName = "690"

        oParamLong = oParamsLong.Add("TargetVA", ParamLogicalNameType.Profile)
        oParamLong.DisplayName = "691"


    End Sub
    ' ����GetInputParametersImplement������
    Protected Overrides Sub GetInputParametersImplement(ByVal corridorState As CorridorState)
        MyBase.GetInputParametersImplement(corridorState)
        ' '�Ӵ����CorridorState�����л�ȡ����
        Dim oParamsLong As ParamLongCollection
        oParamsLong = corridorState.ParamsLong

        Dim oParamsDouble As ParamDoubleCollection
        oParamsDouble = corridorState.ParamsDouble

        ' 
        oParamsLong.Add(Utilities.Side, SideDefault)
        oParamsDouble.Add("Width", LaneWidthDefault)
        oParamsDouble.Add("Depth", LaneDepthDefault)
        oParamsDouble.Add("Slope", LaneSlopeDefault)
        oParamsLong.Add("TransitionType", HoldOriginalPositionDefault)
    End Sub

    ' ����DrawImplement������

    Protected Overrides Sub DrawImplement(ByVal corridorState As CorridorState)


        ''�Ӵ����CorridorState�����л�ȡ����
        Dim oParamsDouble As ParamDoubleCollection
        oParamsDouble = corridorState.ParamsDouble

        Dim oParamsLong As ParamLongCollection
        oParamsLong = corridorState.ParamsLong

        Dim oParamsAlignment As ParamAlignmentCollection
        oParamsAlignment = corridorState.ParamsAlignment

        Dim oParamsProfile As ParamProfileCollection
        oParamsProfile = corridorState.ParamsProfile

        '---------------------------------------------------------
        ' �ж���໹���Ҳࣨflip������
        Dim vSide As Long
        Try
            vSide = oParamsLong.Value(Utilities.Side)
        Catch
            vSide = SideDefault
        End Try

        Dim dFlip As Double
        dFlip = 1.0#
        If vSide = Utilities.Left Then
            dFlip = -1.0#
        End If

        '---------------------------------------------------------
        ' ������������
        Dim vTransitionType As Long
        Try
            vTransitionType = oParamsLong.Value("TransitionType")
        Catch
            vTransitionType = HoldOriginalPositionDefault
        End Try

        '---------------------------------------------------------
        '�����������Ȳ��� 

        Dim vWidth As Double
        Try
            vWidth = oParamsDouble.Value("Width")
        Catch
            vWidth = LaneWidthDefault
        End Try



        Dim vDepth As Double
        Try
            vDepth = oParamsDouble.Value("Depth")
        Catch
            vDepth = LaneDepthDefault
        End Try



        Dim vSlope As Double
        Try
            vSlope = oParamsDouble.Value("Slope")
        Catch
            vSlope = LaneSlopeDefault
        End Try

        '-------------------------------------------------------
        ' �汾�жϣ������������Ҫ�Ĳ���
        Dim sVersion As String
        sVersion = Utilities.GetVersion(corridorState)
        If sVersion <> Utilities.R2005 Then
            '����Ҫ�ı�
        Else
            'R2005
            'ת���¶� %slopeΪ����ֵ
            vSlope = vSlope / 100
        End If

        '---------------------------------------------------------
        ' ����û�����
        If vWidth <= 0 Then
            Utilities.RecordError(corridorState, CorridorError.ValueShouldNotBeLessThanOrEqualToZero, "Width", "BasicLaneTransition")
            vWidth = LaneWidthDefault
        End If

        If vDepth <= 0 Then
            Utilities.RecordError(corridorState, CorridorError.ValueShouldNotBeLessThanOrEqualToZero, "Depth", "BasicLaneTransition")
            vDepth = LaneDepthDefault
        End If


        '���㵱ǰ��·�ߺ�װ��ƫ��
        Dim oCurrentAlignmentId As ObjectId
        Dim oOrigin As New PointInMem

        Utilities.GetAlignmentAndOrigin(corridorState, oCurrentAlignmentId, oOrigin)

        '---------------------------------------------------------
        ' ����㡢���Ӻ����͵ı���
        Dim sPointCodeArray(0 To 4, 0) As String
        Dim sLinkCodeArray(0 To 2, 0 To 1) As String
        Dim sShapeCodeArray(0 To 1) As String

        FillCodesFromTable(sPointCodeArray, sLinkCodeArray, sShapeCodeArray)

        '---------------------------------------------------------
        ' ��ȡ���������·�ߺ�������

        Dim oHAId As ObjectId '·�߶��� Id
        Dim oVAId As ObjectId '��������� Id

        Dim dOffsetToTargetHA As Double
        Dim dOffsetElev As Double

        If corridorState.Mode = CorridorMode.Layout Then
            vTransitionType = TransitionTypes.kHoldOffsetAndElevation
        End If

        Dim dTempStation As Double
        Dim dTempOffset As Double
        Dim oProfileAlignmentId As ObjectId

        Select Case vTransitionType
            Case TransitionTypes.kHoldOffsetAndElevation

            Case TransitionTypes.kHoldElevationChangeOffset
                '
                Try
                    oHAId = oParamsAlignment.Value("TargetHA")
                Catch
                    Utilities.RecordError(corridorState, CorridorError.ParameterNotFound, "Edge Offset", "BasicLaneTransition")
                    'Exit Sub
                End Try
                '��ȡ�� targetHA��ƫ����
                If False = Utilities.CalcAlignmentOffsetToThisAlignment(oCurrentAlignmentId, corridorState.CurrentStation, oHAId, Utilities.GetSide(vSide), dOffsetToTargetHA, dTempStation) Then
                    Utilities.RecordWarning(corridorState, CorridorError.LogicalNameNotFound, "TargetHA", "BasicLaneTransition")
                    dOffsetToTargetHA = vWidth + oOrigin.Offset
                    dTempStation = corridorState.CurrentStation
                End If


            Case TransitionTypes.kHoldGradeChangeOffset
                'oHA must exist
                Try
                    oHAId = oParamsAlignment.Value("TargetHA")
                Catch
                    Utilities.RecordError(corridorState, CorridorError.ParameterNotFound, "Edge Offset", "BasicLaneTransition")
                    'Exit Sub
                End Try
                '��ȡ�� targetHA��ƫ����
                If False = Utilities.CalcAlignmentOffsetToThisAlignment(oCurrentAlignmentId, corridorState.CurrentStation, oHAId, Utilities.GetSide(vSide), dOffsetToTargetHA, dTempStation) Then
                    Utilities.RecordWarning(corridorState, CorridorError.LogicalNameNotFound, "TargetHA", "BasicLaneTransition")
                    dOffsetToTargetHA = vWidth + oOrigin.Offset
                    dTempStation = corridorState.CurrentStation
                End If

            Case TransitionTypes.kHoldOffsetChangeElevation
                'oVA must exist
                Try
                    oVAId = oParamsProfile.Value("TargetVA")
                Catch
                    Utilities.RecordError(corridorState, CorridorError.ParameterNotFound, "Edge Elevation", "BasicLaneTransition")
                    'Exit Sub
                End Try

                Dim db As Database = HostApplicationServices.WorkingDatabase
                Dim tm As DBTransactionManager = db.TransactionManager
                Dim oProfile As Profile = Nothing

                '��ȡ�ݶ����ϵĸ߳�
                Try
                    oProfile = tm.GetObject(oVAId, OpenMode.ForRead, False, False)
                    oProfileAlignmentId = oProfile.AlignmentId
                Catch
                End Try

                If False = Utilities.CalcAlignmentOffsetToThisAlignment(oCurrentAlignmentId, corridorState.CurrentStation, oProfileAlignmentId, Utilities.GetSide(vSide), dTempOffset, dTempStation) Then
                    Utilities.RecordWarning(corridorState, CorridorError.LogicalNameNotFound, "TargetHA", "BasicLaneTransition")
                    dOffsetElev = corridorState.CurrentElevation + vWidth * vSlope
                Else
                    dOffsetElev = oProfile.ElevationAt(dTempStation)
                End If


            Case TransitionTypes.kChangeOffsetAndElevation
                ' oHA �� oVA �������
                Try
                    oHAId = oParamsAlignment.Value("TargetHA")
                Catch
                    Utilities.RecordError(corridorState, CorridorError.ParameterNotFound, "Edge Offset", "BasicLaneTransition")
                    'Exit Sub
                End Try

                Try
                    oVAId = oParamsProfile.Value("TargetVA")
                Catch
                    Utilities.RecordError(corridorState, CorridorError.ParameterNotFound, "Edge Elevation", "BasicLaneTransition")
                    'Exit Sub
                End Try


                '��ȡ�ݶ����ϵĸ߳�
                Dim db As Database = HostApplicationServices.WorkingDatabase
                Dim tm As DBTransactionManager = db.TransactionManager
                Dim oProfile As Profile = Nothing

                Try
                    oProfile = tm.GetObject(oVAId, OpenMode.ForRead, False, False)
                    oProfileAlignmentId = oProfile.AlignmentId
                Catch
                End Try

                If False = Utilities.CalcAlignmentOffsetToThisAlignment(oCurrentAlignmentId, corridorState.CurrentStation, oProfileAlignmentId, Utilities.GetSide(vSide), dTempOffset, dTempStation) Then
                    Utilities.RecordWarning(corridorState, CorridorError.LogicalNameNotFound, "TargetHA", "BasicLaneTransition")
                    dOffsetElev = corridorState.CurrentElevation + vWidth * vSlope
                Else
                    dOffsetElev = oProfile.ElevationAt(dTempStation)
                End If

                '��ȡ�� targetHA��ƫ����
                If False = Utilities.CalcAlignmentOffsetToThisAlignment(oCurrentAlignmentId, corridorState.CurrentStation, oHAId, Utilities.GetSide(vSide), dOffsetToTargetHA, dTempStation) Then
                    Utilities.RecordWarning(corridorState, CorridorError.LogicalNameNotFound, "TargetHA", "BasicLaneTransition")
                    dOffsetToTargetHA = vWidth + oOrigin.Offset
                End If
        End Select

        '---------------------------------------------------------
        ' ����������
        Dim corridorPoints As PointCollection
        corridorPoints = corridorState.Points

        Dim dX As Double
        Dim dy As Double

        dX = 0.0#
        dy = 0.0#
        Dim oPoint1 As Point
        oPoint1 = corridorPoints.Add(dX, dy, "")

        ' ���㳵�������λ�� 
        Select Case vTransitionType

            Case TransitionTypes.kHoldOffsetAndElevation

                ' 
                dX = vWidth
                dy = Abs(vWidth) * vSlope

            Case TransitionTypes.kHoldElevationChangeOffset

                ' 
                'dX = Abs(dOffsetToTargetHA - corridorState.CurrentSubassemblyOffset)
                dX = Abs(dOffsetToTargetHA - oOrigin.Offset)
                dy = Abs(vWidth) * vSlope

            Case TransitionTypes.kHoldGradeChangeOffset

                dX = Abs(dOffsetToTargetHA - oOrigin.Offset)
                dy = Abs(dX) * vSlope

            Case TransitionTypes.kHoldOffsetChangeElevation

                ' 
                dX = vWidth
                'dY = dOffsetElev - corridorState.CurrentSubassemblyElevation
                dy = dOffsetElev - oOrigin.Elevation

            Case TransitionTypes.kChangeOffsetAndElevation

                ' 
                dX = Abs(dOffsetToTargetHA - oOrigin.Offset)
                dy = dOffsetElev - oOrigin.Elevation

        End Select

        Dim oPoint2 As Point
        oPoint2 = corridorPoints.Add(dX * dFlip, dy, "")

        dX = dX - 0.001
        dy = dy - vDepth
        Dim oPoint3 As Point
        oPoint3 = corridorPoints.Add(dX * dFlip, dy, "")

        dX = 0.0#
        dy = -vDepth
        Dim oPoint4 As Point
        oPoint4 = corridorPoints.Add(dX, dy, "")

        Utilities.AddCodeToPoint(1, corridorPoints, oPoint1.Index, sPointCodeArray)
        Utilities.AddCodeToPoint(2, corridorPoints, oPoint2.Index, sPointCodeArray)
        Utilities.AddCodeToPoint(3, corridorPoints, oPoint3.Index, sPointCodeArray)
        Utilities.AddCodeToPoint(4, corridorPoints, oPoint4.Index, sPointCodeArray)


        '---------------------------------------------------------
        ' ������������

        Dim oCorridorLinks As LinkCollection
        oCorridorLinks = corridorState.Links

        Dim oPoint(1) As Point
        Dim oLink(3) As Link

        oPoint(0) = oPoint1
        oPoint(1) = oPoint2
        oLink(0) = oCorridorLinks.Add(oPoint, "") 'L1

        oPoint(0) = oPoint2
        oPoint(1) = oPoint3
        oLink(1) = oCorridorLinks.Add(oPoint, "") 'L2

        oPoint(0) = oPoint3
        oPoint(1) = oPoint4
        oLink(2) = oCorridorLinks.Add(oPoint, "") 'L3

        oPoint(0) = oPoint4
        oPoint(1) = oPoint1
        oLink(3) = oCorridorLinks.Add(oPoint, "") 'L4

        Utilities.AddCodeToLink(1, oCorridorLinks, oLink(0).Index, sLinkCodeArray)
        Utilities.AddCodeToLink(2, oCorridorLinks, oLink(2).Index, sLinkCodeArray)

        '---------------------------------------------------------
        ' ����װ������

        Dim corridorShapes As ShapeCollection
        corridorShapes = corridorState.Shapes


        corridorShapes.Add(oLink, sShapeCodeArray(1))

    End Sub

    Protected Sub FillCodesFromTable(ByVal sPointCodeArray(,) As String, ByVal sLinkCodeArray(,) As String, ByVal sShapeCodeArray() As String)
        sPointCodeArray(1, 0) = Codes.Crown.Code
        sPointCodeArray(2, 0) = Codes.ETW.Code
        sPointCodeArray(3, 0) = Codes.ETWSubBase.Code 'P4
        sPointCodeArray(4, 0) = Codes.CrownSubBase.Code 'P3

        sLinkCodeArray(1, 0) = Codes.Top.Code
        sLinkCodeArray(1, 1) = Codes.Pave.Code
        sLinkCodeArray(2, 0) = Codes.Datum.Code
        sLinkCodeArray(2, 1) = Codes.SubBase.Code

        sShapeCodeArray(1) = Codes.Pave1.Code
    End Sub
End Class
