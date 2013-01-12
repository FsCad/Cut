using System;
using System.Collections.Generic;
using System.Text;
using Inventor;
using System.Windows.Forms;

namespace Chapter5
{
    class MySketch
    {
        public void CreateSketch(Inventor.Application ThisApplication)
        {
            if (ThisApplication.ActiveEditObject is Sketch == false)
            {
                MessageBox.Show("A sketch must be active.");
            }
            Sketch sketch;
            sketch = (Sketch)ThisApplication.ActiveEditObject;

            TransientGeometry transGeom;
            transGeom = ThisApplication.TransientGeometry;

            Point2d coord1;
            coord1 = transGeom.CreatePoint2d(0, 0);

            Point2d coord2;
            coord2 = transGeom.CreatePoint2d(5, 0);

            System.Array Line = System.Array.CreateInstance(typeof(SketchLine), 4);
            SketchLine[] lines = Line as SketchLine[];
            lines[0] = sketch.SketchLines.AddByTwoPoints(coord1, coord2);

            coord1 = transGeom.CreatePoint2d(5, 3);
            lines[1] = sketch.SketchLines.AddByTwoPoints(lines[0].EndSketchPoint, coord1);

            coord1 = transGeom.CreatePoint2d(4, 3);
            coord2 = transGeom.CreatePoint2d(4, 4);
            SketchArc sketchArc;
            sketchArc = sketch.SketchArcs.AddByCenterStartEndPoint(coord1,lines[1].EndSketchPoint,coord2,true);

            coord1 = transGeom.CreatePoint2d(0, 4);
            lines[2] = sketch.SketchLines.AddByTwoPoints(sketchArc.EndSketchPoint,coord1);

            lines[3] = sketch.SketchLines.AddByTwoPoints(lines[0].StartSketchPoint, lines[2].EndSketchPoint);

            //���Լ��
            sketch.GeometricConstraints.AddHorizontal((SketchEntity)lines[0], true);
            sketch.GeometricConstraints.AddPerpendicular((SketchEntity)lines[0], (SketchEntity)lines[1], false, true);
            sketch.GeometricConstraints.AddTangent((SketchEntity)lines[1], (SketchEntity)sketchArc, null);
            sketch.GeometricConstraints.AddTangent((SketchEntity)lines[2], (SketchEntity)sketchArc, null);
            sketch.GeometricConstraints.AddParallel((SketchEntity)lines[0], (SketchEntity)lines[2], true, false);
            sketch.GeometricConstraints.AddParallel((SketchEntity)lines[3], (SketchEntity)lines[1], true, false);

            //������ͼ
            PartDocument partDoc;
            partDoc = (PartDocument)ThisApplication.ActiveDocument;
            for (int i = 1; i <= partDoc.ComponentDefinition.Sketches.Count; i++ )
            {
                MessageBox.Show(partDoc.ComponentDefinition.Sketches[i].Name);
            }

            //ͨ�����Ʒ��ʲ�ͼ
            Sketch sketch1;
            sketch1 = (Sketch)partDoc.ComponentDefinition.Sketches["��ͼ1"];
            MessageBox.Show(sketch1.Name);

        }

        public void CreateModel(Inventor.Application ThisApplication)
        {
            //���õ��㲿�����������
            PartComponentDefinition partCompDef;
            partCompDef = ((PartDocument)ThisApplication.ActiveDocument).ComponentDefinition;

            //�����²�ͼ
            PlanarSketch newSketch;
            newSketch = partCompDef.Sketches.Add(partCompDef.WorkPlanes[3], false);

            //��һ������
            TransientGeometry transGeom;
            transGeom = ThisApplication.TransientGeometry;
            newSketch.SketchLines.AddAsTwoPointRectangle(transGeom.CreatePoint2d(0, 0), transGeom.CreatePoint2d(5, 3));

            //��������
            Profile profile;
            profile = newSketch.Profiles.AddForSolid(true, null, null);

            //����һ����������
            ExtrudeFeature firstExtrude;
            firstExtrude = partCompDef.Features.ExtrudeFeatures.AddByDistanceExtent(profile, 2, PartFeatureExtentDirectionEnum.kPositiveExtentDirection, PartFeatureOperationEnum.kJoinOperation, null);
  
            //ʹ�����еĹ������ԭ�������������ı��洴����ͼ
            PlanarSketch sketch;
            sketch = partCompDef.Sketches.AddWithOrientation(firstExtrude.EndFaces[1], partCompDef.WorkAxes[1],
                true, true, partCompDef.WorkPoints[1],false);

            //��һ������
            sketch.SketchLines.AddAsTwoPointRectangle(transGeom.CreatePoint2d(1, 1), transGeom.CreatePoint2d(4, 2));

            //��������
            Profile myProfile;
            myProfile = sketch.Profiles.AddForSolid(true, null, null);

            //������������
            ExtrudeFeature secondExtrude;
            secondExtrude = partCompDef.Features.ExtrudeFeatures.AddByDistanceExtent(myProfile, 0.75, PartFeatureExtentDirectionEnum.kNegativeExtentDirection,��PartFeatureOperationEnum.kCutOperation,��null);
        }
    }
}
