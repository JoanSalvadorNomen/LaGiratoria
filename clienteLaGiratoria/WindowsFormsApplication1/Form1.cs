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

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        Socket server;

        string jugador;
        string invitado;
        bool listacargada = false;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            button4.Visible = false;
            TablaUsuariosConectados.Visible = false;
            label1.Visible = false;
            label2.Visible = false;
            groupBox1.Visible = false;
            nombre.Visible = false;
            contrasena.Visible = false;
            button3.Visible = false;
            button5.Visible = false;
            label4.Visible = false;


        }

        private void button1_Click(object sender, EventArgs e)
        {
            //Creamos un IPEndPoint con el ip del servidor y puerto del servidor 
            //al que deseamos conectarnos
            IPAddress direc = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direc, 9050);


            //Creamos el socket 
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);//Intentamos conectar el socket
                button3.Visible = true;
                label1.Visible = true;
                label2.Visible = true;
                nombre.Visible = true;
                contrasena.Visible = true;
                button4.Visible = true;
                button1.Visible = false;
                TablaUsuariosConectados.Visible = true;
                button5.Visible = true;

            }
            catch (SocketException ex)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            }


        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (Registros.Checked)
            {
                string mensaje = "1/" + nombre.Text;
                // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show("Los jugadores son: " + mensaje);

            }
            else if (Partidas.Checked)
            {
                string mensaje = "2/" + nombre.Text;
                // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show("Has jugado las partidas siguientes: " + mensaje);

            }
            else
            {
                // Enviamos nombre y altura
                string mensaje = "3/" + nombre.Text + "/" + fechaBox.Text;
                // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show("Tus ganancias son:" + mensaje);
            }


        }

        private void button3_Click(object sender, EventArgs e)
        {
            //Mensaje de desconexión
            string mensaje = "0/" + nombre.Text;

            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            // Nos desconectamos
            this.BackColor = Color.Gray;
            server.Shutdown(SocketShutdown.Both);
            server.Close();
            button3.Visible = false;
            label1.Visible = false;
            label2.Visible = false;
            nombre.Visible = false;
            contrasena.Visible = false;
            button4.Visible = false;
            button1.Visible = true;
            TablaUsuariosConectados.Visible = false;
            button5.Visible = false;
            groupBox1.Visible = false;
            label4.Visible = false;



        }

        private void button4_Click(object sender, EventArgs e)
        {
            jugador = nombre.Text;
            string mensaje = "4/" + nombre.Text + "/" + contrasena.Text;
            // Enviamos al servidor el nombre tecleado
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];

            if (mensaje == "Si")
            {
                groupBox1.Visible = true;
                label1.Visible = false;
                label2.Visible = false;
                nombre.Visible = false;
                contrasena.Visible = false;
                button4.Visible = false;
                label4.Visible = true;
                label4.Text = "¡Hola " + nombre.Text + "!";
            }
            else
                MessageBox.Show("Contraseña incorrecta");
        }


        private void button5_Click(object sender, EventArgs e)
        {
            string mensaje = "5/";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];

            string[] jugadores = mensaje.Split(',');
            int jugadorantiguo = TablaUsuariosConectados.RowCount;
            int jugadornuevo = Convert.ToInt32(jugadores[0]);



            if (listacargada == true)
            {

                for (int i = 0; i < (jugadorantiguo - 1); i++)
                {
                    TablaUsuariosConectados.Rows[i].Cells[0].Value = jugadores[i + 1];
                    TablaUsuariosConectados.Refresh();
                }

                TablaUsuariosConectados.Refresh();


                if (jugadornuevo < jugadorantiguo)
                {
                    for (int i = jugadornuevo; i < jugadorantiguo; i++)
                    {
                        TablaUsuariosConectados.Rows.RemoveAt(jugadornuevo);
                        TablaUsuariosConectados.Refresh();
                    }

                    TablaUsuariosConectados.Refresh();
                }


                else if (jugadornuevo > jugadorantiguo)
                {
                    for (int i = jugadorantiguo; i < jugadornuevo; i++)
                    {

                        string nombrenuevo = Convert.ToString(jugadores[i + 1]);


                        TablaUsuariosConectados.Rows.Add(nombrenuevo);
                        TablaUsuariosConectados.Refresh();
                    }
                }
            }


            else if (listacargada == false)
            {
                for (int i = 1; i < (Convert.ToInt32(jugadores[0]) + 1); i++)
                {

                    string nombrenuevo = Convert.ToString(jugadores[i]);


                    TablaUsuariosConectados.Rows.Add(nombrenuevo);
                    TablaUsuariosConectados.Refresh();
                    listacargada = true;
                }
            }

        }
        

        private void TablaUsuariosConectados_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (TablaUsuariosConectados.CurrentCell.Value != null && Convert.ToString(TablaUsuariosConectados.CurrentCell.Value) != jugador)
            {
                string mensaje = "6/ENVIAR/";
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje + Convert.ToString(TablaUsuariosConectados.CurrentCell.Value));
                server.Send(msg);

            }
            else 
            {
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                string mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show(mensaje);
                string[] trozos = mensaje.Split('/');
                string peticion = trozos[0];
                string contrincante = trozos[1];

                if (jugador == contrincante)
                {
                    if (peticion == "RECIBIR")
                    {
                        MessageBox.Show("Te invita " + invitado + " a jugar.");
                        //if (Response == SI)
                        //{
                        //    mensaje = "6/SI/" + invitado;
                        //    byte[] msg3 = System.Text.Encoding.ASCII.GetBytes(mensaje);
                        //    server.Send(msg3);
                        //}
                        //else if (Response == NO)
                        //{
                        //   mensaje = "6/NO/" + invitado;
                        //    byte[] msg3 = System.Text.Encoding.ASCII.GetBytes(mensaje);
                        //    server.Send(msg3);
                        //}
                    }
                    else if (peticion == "SI")
                    {
                        MessageBox.Show(invitado + "quiere ganar dinero también.");
                    }
                    else if (peticion == "NO")
                    {
                        MessageBox.Show(invitado + "no quiere ganar dinero, invita a otro.");
                    }
                    else
                    {
                        MessageBox.Show("No te ha invitado nadie notas");
                    }
                }
            }
           
        }
    }

}
