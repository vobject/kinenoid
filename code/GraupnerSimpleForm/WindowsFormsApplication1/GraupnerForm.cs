using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

/*
   124,  // Servo #10 - GJ_RIGHT_HIPSIDE
   125,  // Servo #11 - GJ_RIGHT_HIPUP
   140,  // Servo #12 - GJ_RIGHT_HIPROT
   254,  // Servo #13 - GJ_RIGHT_KNEE
   110,  // Servo #14 - GJ_RIGHT_FOOT
   132,  // Servo #15 - GJ_RIGHT_FOOTSIDE
   120,  // Servo #16 - GJ_RIGHT_SHOULDER
   128,  // Servo #17 - GJ_RIGHT_ARM
   125,  // Servo #18 - GJ_RIGHT_HAND
   128,  // Servo #19 - GJ_HEAD
*/

namespace WindowsFormsApplication1
{
    public partial class GraupnerForm : Form
    {
        private byte[] servosData = new byte[] { 124, 130, 145, 20, 140, 140, 150, 120, 135, 124, 125, 140, 254, 110, 132, 120, 128, 125, 128, 0 };
//        private byte[] servosData = new byte[] { 124,	130,	145,	20,	140,	140,	150,	120,	135,		124,	125,	140,	254,	110,	132,	120,	128,	125,		128, 0}; // 0 
//        private byte[] servosData = new byte[] { 134,	130,	145,	20,	140,	130,	150,	120,	135,		124,	125,	140,	254,	110,	152,	120,	128,	125,		128, 1}; // 1 
//        private byte[] servosData = new byte[] { 134,	150,	145,	20,	154,	130,	150,	120,	135,		124,	115,	140,	254,	100,	146,	120,	128,	125,		128, 0}; // 2 
//        private byte[] servosData = new byte[] { 124,	150,	145,	20,	154,	170,	150,	120,	135,		134,	115,	140,	254,	100,	132,	120,	128,	125,		128, 1}; // 3 
//       private byte[] servosData = new byte[] { 124,	120,	145,	20,	130,	160,	150,	120,	135,		114,	105,	140,	254,	90,	142,	120,	128,	125,		128, 0}; // 4 ???
//        private byte[] servosData = new byte[] { 134,	120,	145,	20,	130,	140,	150,	120,	135,		124,	115,	140,	254,	 90,	118,	120,	128,	125,		128, 1}; // 5 
//         private byte[] servosData = new byte[] { 134,	150,	145,	20,	154,	130,	150,	120,	135,		124,	135,	140,	254,	120,	118,	120,	128,	125,		128, 0}; // 6 
        private byte[] buffer = new byte[20];
        private byte[] enable = new byte[] { 0xff};
        private SerialPort serialPort = null;

        public GraupnerForm()
        {
            InitializeComponent();
            try
            {
                init();
            }
            catch (System.Exception ex)
            {  }
            buffer = servosData;
        }

        private void init()
        {
            serialPort = new SerialPort("COM5", 38400, Parity.None, 8, StopBits.One);
            serialPort.Open();
//            serialPort.Write(enable, 0, enable.Length);
//            serialPort.Write(buffer, 0, buffer.Length);
        }

        private void setDefault()
        {
            int i = 0;
            foreach ( Control con in this.Controls.OfType<NumericUpDown>())
            {
                int.TryParse(con.Name.Remove(0, con.Name.IndexOf("_") + 1), out i);
                (con as NumericUpDown).Value = servosData[i];
            }
        }

        private void send()
        {
            serialPort.Write(new byte[] { 0xff }, 0, 1);
            serialPort.Write(buffer, 0, buffer.Length);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            setDefault();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            getDataFromControls();
            send();
        }

        private void getDataFromControls()
        {
            int i = 0;
            foreach (Control con in this.Controls.OfType<NumericUpDown>())
            {
                int.TryParse(con.Name.Remove(0, con.Name.IndexOf("_") + 1), out i);
                servosData[i] = (byte)(con as NumericUpDown).Value;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }


    }
}


