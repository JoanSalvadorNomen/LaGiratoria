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
    public partial class Form3 : Form
    {
        Socket server;
        Thread threadform4;
        int saldo;
        int conteo;
        int conteo2;
        string Jugador;
        string invitado;
        string numeroAleatorio;
        List<Form4> ListaForm4 = new List<Form4>();
        delegate void DelegadoParaEscribir(string respuesta);
        public Form3(Socket server, int saldomio, string jugador, string invitado)
        {
            InitializeComponent();
            this.server = server;
            conteo = 0;
            conteo2 = 0;
            saldo = saldomio;
            this.Jugador = jugador;
            this.invitado = invitado;
            label6.Text = "Estas jugando contra: " + invitado;
        }
        private void EscribeLabel4(string lb4) //delagado para escribir el saldo
        {
            saldo = 1000 + Convert.ToInt32(lb4);
            label4.Text = "Saldo: " + saldo;
        }
        private void Form3_Load(object sender, EventArgs e)
        {
            timer1.Enabled = true;
            label5.Text = "Tienen 20 segundos para hacer sus apuestas 20s";
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            conteo++;
            label5.Text = "Tienen 20 segundos para hacer sus apuestas "+ (20-conteo) + "s";
            if (conteo >= 20)
            {
                conteo = -8;
                Form4 frm = new Form4(server, invitado, saldo, numeroAleatorio);
                frm.Show();
            }
            else if (conteo == 0)
            {
                string mensaje = "14/";
                //Enviamos al servidor la jugada
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else if (conteo == 1)
            {
                string mensaje2 = "14/";
                // Enviamos al servidor la jugada
                byte[] msg3 = System.Text.Encoding.ASCII.GetBytes(mensaje2);
                server.Send(msg3);
            }
            else if (conteo == 2)
            {
                string mensaje = "12/";
                // Enviamos al servidor la jugada
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }    
            else if (conteo == 4)
            {
                string mensaje = "13/";
                //Enviamos al servidor la jugada
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else if (conteo == 19)
            {
                string mensaje = "15/" + Jugador + "/" + invitado;
                // Enviamos al servidor la jugada
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
        }
        private void button1_Click(object sender, EventArgs e) //Boton para apostar
        {
            if (saldo >= 0 && Convert.ToInt32(cantidad.Text) <= saldo)
            {
                saldo = saldo - Convert.ToInt32(cantidad.Text);
                label4.Text = "Saldo: " + saldo;
                string mensaje = "10/" + casilla.Text + "/" + cantidad.Text;
                // Enviamos al servidor la jugada
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
                MessageBox.Show("Saldo insuficiente");
        }
        public void ActualizarSaldo(string saldoactualizado)
        {
            this.Invoke(new DelegadoParaEscribir(EscribeLabel4),
                            new object[] { saldoactualizado });
        }
        public void ActualizarNumero(string num)
        {
            numeroAleatorio = num;
        }
    }
}
