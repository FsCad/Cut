using System;
using System.Collections.Generic;
using System.Text;
using Inventor;
using System.Windows.Forms;

namespace Chapter8
{
    class MySelectSet
    {
        public void ShowSurfaceArea(Inventor.Application ThisApplication)
        {
            //���õ������ĵ���ѡ�񼯵�����
            SelectSet selectSet;
            selectSet = ThisApplication.ActiveDocument.SelectSet;

            //ȷ��ֻѡ����һ��ͼԪ
            if(selectSet.Count == 1)
            {
                //ȷ��ѡ����һ������
                if (selectSet[1].GetType() is Face == false)
                {
                    //���õ���ѡ���������
                    Face face;
                    face = (Face)selectSet[1];

                    //��ʾ��ѡ��������
                    MessageBox.Show(string.Format("Surface area:  {0}  cm^2", face.Evaluator.Area));
                }
                else
                {
                    MessageBox.Show("You must select a single face.");
                }
            }
            else
            {
                MessageBox.Show("You must select a single face.");
            }
        }
    }
}
