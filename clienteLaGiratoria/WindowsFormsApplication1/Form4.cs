using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;
using System.Diagnostics.Eventing.Reader;
using System.Runtime.Remoting.Channels;

namespace WindowsFormsApplication1
{
    public partial class Form4 : Form
    {
        int conteo;
        Socket server;
        delegate void DelegadoParaEscribir(string text);
        string invitado;
        string numeroAleatorio;
        int saldo;
        int ganancias;
        public Form4(Socket server, string invitado, int saldomio, string numerin)
        {
            InitializeComponent();
            this.server = server;
            this.invitado = invitado;
            this.saldo = saldomio;
            conteo = 0;
            this.numeroAleatorio = numerin;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            conteo++;
            if (conteo <= 60)
            {
                pictureBox1.Image.RotateFlip(RotateFlipType.Rotate90FlipNone);
                pictureBox1.Invalidate();
            }
            else if (conteo >= 60 && conteo < 63)
            {
                label2.Text = numeroAleatorio;
                label2.Visible = true;
            }
            if (conteo > 80)
            {
               timer1.Enabled = false;
               this.Close();
            }

        }
        private void Form4_Load(object sender, EventArgs e)
        {
               timer1.Enabled = true;
               label2.Visible = false;
        }
    }
}
