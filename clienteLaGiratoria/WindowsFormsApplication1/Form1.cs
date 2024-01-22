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
    public partial class Form1 : Form
    {
        Socket server;
        Thread atender, threadform3, threadform4;
        List<Form3> ListaForm3 = new List<Form3>();
        List<Form4> ListaForm4 = new List<Form4>();
        delegate void DelegadoParaBoolear(bool boolean);
        delegate void DelegadoParaEscribir(string text);
        delegate void DelegadoParaNumerar(int num);


        string jugador;
        string invitado;
        bool listacargada = false;
        bool atendiendo = true;



        public Form1()
        {
            InitializeComponent();
        }
        private void PonGroupBox1(bool groupbox1)
        {
            groupBox1.Visible = groupbox1;
        }
        private void PonLabel1(bool lb1)
        {
            label1.Visible = lb1;
        }
        private void PonLabel2(bool lb2)
        {
            label2.Visible = lb2;
        }
        private void PonLabel4(bool lb4)
        {
            label4.Visible = lb4;
        }
        private void EscribeLabel4(string lb4)
        {
            label4.Text = lb4;
        }
        private void PonButton6(bool bu6)
        {
            button6.Visible = bu6;
        }
        private void HabilitaButton6(bool bu6)
        {
            button6.Enabled = bu6;
        }
        private void PonButton8(bool bu8)
        {
            button8.Visible = bu8;
        }
        private void PonButton9(bool bu9)
        {
            button9.Visible = bu9;
        }
        private void PonButton4(bool bu4)
        {
            button4.Visible = bu4;
        }
        private void PonButton3(bool bu3)
        {
            button3.Visible = bu3;
        }
        private void PonButton1(bool bu1)
        {
            button1.Visible = bu1;
        }
        private void PonNombre(bool nom)
        {
            nombre.Visible = nom;
        }
        private void PonContrasena(bool cont)
        {
            contrasena.Visible = cont;
        }
        private void PonTabla(bool tab)
        {
            TablaUsuariosConectados.Visible = tab;
        }
        private void EscribeTabla(string add)
        {
            TablaUsuariosConectados.Rows.Add(add);
            TablaUsuariosConectados.Refresh();
        }
        private void QuitaTabla(int quita)
        {
            TablaUsuariosConectados.Rows.RemoveAt(quita);
            TablaUsuariosConectados.Refresh();
        }
        private void GenteTabla(string gente)
        {
            int i = 0;
            TablaUsuariosConectados.Rows[i].Cells[0].Value = gente;
            TablaUsuariosConectados.Refresh();
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
            button8.Visible = false;
            button6.Visible = false;
            button9.Visible = false;
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
                button8.Visible = true;
                button9.Visible = true;
                button1.Visible = false;
                TablaUsuariosConectados.Visible = true;
                atendiendo = true;
                ThreadStart ts = delegate { AtenderServidor(); };
                atender = new Thread(ts);
                atender.Start();
            }
 
            catch (SocketException ex)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            }
        }

        private void button2_Click(object sender, EventArgs e) //Boton para enviar las consultas
        {
            if (Registros.Checked) //con quien he jugado
            {
                string mensaje = "1/" + jugador; 
                // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }

            else if (Partidas.Checked) //resultados
            {
                string mensaje = "2/" + jugador + "/" + contrincant.Text;
                // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }

            else //partida jugada segun el dia
            {
                string mensaje = "3/" + nombre.Text + "/" + fechaBox.Text;
                // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
        }

        private void button3_Click(object sender, EventArgs e) //para desconectarse
        {
            string mensaje = "0/" + nombre.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

        private void button4_Click(object sender, EventArgs e) //iniciar sesion
        {
            jugador = nombre.Text;
            string mensaje = "4/" + nombre.Text + "/" + contrasena.Text;
            // Enviamos al servidor el nombre tecleado
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

        private void button6_Click_1(object sender, EventArgs e) //invitar a alguien
        {
            //Comprobamos si ya hay algún jugador seleccionado de la tabla de conectados y no es el nombre del usuario logueado en este cliente
            if (TablaUsuariosConectados.CurrentCell.Value != null && Convert.ToString(TablaUsuariosConectados.CurrentCell.Value) != jugador)
            {
                //Enviamos un mensaje al servidor para solicitar una partida
                string mensaje = "6/ENVIAR/";
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje + Convert.ToString(TablaUsuariosConectados.CurrentCell.Value));
                server.Send(msg);

                //Deshabilitamos el botón para poder evitar más de una invitación a la vez
                this.Invoke(new DelegadoParaBoolear(HabilitaButton6),
                new object[] { false });
            }
            //Indicamos al usuario de seleccionar un jugador para poder realizar el proceso de invitación
            else
            {
                MessageBox.Show("No has seleccionado ningún jugador válido para invitar, selecciona a uno para empezar la partida");
            }
        }

        private void button8_Click(object sender, EventArgs e) //Registrarse
        {
            string mensaje = "8/" + nombre.Text + "/" + contrasena.Text;
            // Enviamos al servidor el nombre tecleado
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

        private void button9_Click(object sender, EventArgs e) //Darse de baja
        {
            string mensaje = "9/" + nombre.Text + "/" + contrasena.Text;
            // Enviamos al servidor el nombre tecleado
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

    

        public void Invitaciones(string peticion, string jugador, string contrincante) //Funcion para gestionar las invitaciones
        {
            string mensaje;

            if (peticion == "RECIBIR")
            {
                DialogResult PeticionDuelo = MessageBox.Show("Has recibido una solicitud de partida por parte de " + contrincante + ", ¿deseas aceptar su petición?", "Invitación recibida", MessageBoxButtons.YesNo);
                if (PeticionDuelo == DialogResult.Yes)
                {
                    //Enviamos un mensaje al servidor para indicar de que aceptamos la partida
                    mensaje = "6/ACEPTAR/" + contrincante;
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    ThreadStart ts = delegate
                    {
                        Form3 frm = new Form3(server, 1000, jugador, contrincante);
                        ListaForm3.Add(frm);
                        frm.ShowDialog();
                    };
                    threadform3 = new Thread(ts);
                    threadform3.Start();
                }
                else if (PeticionDuelo == DialogResult.No)
                {
                    //Enviamos un mensaje al servidor para indicar de que rechazamos la partida
                    mensaje = "6/RECHAZAR/" + contrincante;
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);
                }
            }
            else if (peticion == "SI")
            {
                ThreadStart ts = delegate
                {
                    Form3 frm = new Form3(server, 1000, jugador, Convert.ToString(TablaUsuariosConectados.CurrentCell.Value));
                    ListaForm3.Add(frm);
                    frm.ShowDialog();
                };
                threadform3 = new Thread(ts);
                threadform3.Start();
                this.Invoke(new DelegadoParaBoolear(HabilitaButton6),
                new object[] { true });
            }
            else if (peticion == "NO")
            {
                MessageBox.Show(contrincante + " no quiere ganar dinero, invita a otro.");
                this.Invoke(new DelegadoParaBoolear(HabilitaButton6),
                new object[] { true });
            }

        }
        public void AtenderServidor() //El bucle para interpretar los mensajes del servidor
        {
            while (atendiendo == true)
            {
                //Recibimos mensaje del servidor
                byte[] msg = new byte[500];
                server.Receive(msg);
                string Mensaje = Encoding.ASCII.GetString(msg).Split('\0')[0];
                //Creamos un vector con cada trozo del mensaje recibido (cada cosa que va por cada / es un "trozo")
                string[] Trozos = Mensaje.Split('/');
                int codigo = 0;
                string Respuesta;
                //El primer trozo es el código de la operación realizada
                codigo = Convert.ToInt32(Trozos[0]);
                switch (codigo)
                {
                    case 1:  //Resultados de la primera consulta
                        Respuesta = Trozos[1];
                        MessageBox.Show("Los jugadores son: " + Respuesta);
                        break;

                    case 2:  //Resultados de la segunda consulta
                        Respuesta = Trozos[1];
                        if (Respuesta == "Si")
                        {
                            string gananciaspartida = Trozos[2];
                            string perdidaspartida = Trozos[3];
                            MessageBox.Show("En las partidas contra " + contrincant.Text + ", has ganado " + gananciaspartida + " y has perdido " + perdidaspartida);
                        }
                        else if (Respuesta == "No")
                        {
                            MessageBox.Show("No jugaste ninguna partida con " + contrincant.Text);
                        }
                        break;

                    case 3: //Resultados de la tercera consulta
                        Respuesta = Trozos[1];
                        MessageBox.Show("Ese dia jugaste la(s) partida(s) numero: " + Respuesta);
                        break;

                    case 4: //Despues de iniciar la sesion
                        Respuesta = Trozos[1];
                        if (Respuesta == "Si")
                        {

                            this.Invoke(new DelegadoParaBoolear(PonGroupBox1),
                            new object[] { true });

                            this.Invoke(new DelegadoParaBoolear(PonLabel1),
                            new object[] { false });
                            this.Invoke(new DelegadoParaBoolear(PonLabel2),
                            new object[] { false });
                            this.Invoke(new DelegadoParaBoolear(PonNombre),
                            new object[] { false });
                            this.Invoke(new DelegadoParaBoolear(PonContrasena),
                            new object[] { false });
                            this.Invoke(new DelegadoParaBoolear(PonButton4),
                            new object[] { false });
                            this.Invoke(new DelegadoParaBoolear(PonButton8),
                            new object[] { false });
                            this.Invoke(new DelegadoParaBoolear(PonButton9),
                            new object[] { false });
                            this.Invoke(new DelegadoParaBoolear(PonButton6),
                            new object[] { true });
                            this.Invoke(new DelegadoParaBoolear(PonLabel4),
                            new object[] { true });
                            this.Invoke(new DelegadoParaEscribir(EscribeLabel4),
                            new object[] { "¡Hola " + jugador + "!" });
                        }
                        else
                            MessageBox.Show("Contraseña incorrecta");
                        break;

                    case 5: //Lista de conectados que se actualiza automaticamente
                        Respuesta = Trozos[1];
                        string[] jugadores = Respuesta.Split(',');
                        int jugadorantiguo = TablaUsuariosConectados.RowCount;
                        int jugadornuevo = Convert.ToInt32(jugadores[0]);
                        if (listacargada == true)
                        {

                            for (int i = 0; i < (jugadorantiguo - 1); i++)
                            {
                                this.Invoke(new DelegadoParaEscribir(GenteTabla),
                                new object[] { jugadores[i + 1] });
                            }

                            if (jugadornuevo < jugadorantiguo)
                            {
                                for (int i = 0; i < (jugadorantiguo); i++)
                                {
                                    this.Invoke(new DelegadoParaNumerar(QuitaTabla),
                                    new object[] { 0 });
                                }
                                for (int i = 0; i < jugadornuevo; i++)
                                {
                                    string nombrenuevo = Convert.ToString(jugadores[i + 1]);
                                    this.Invoke(new DelegadoParaEscribir(EscribeTabla),
                                    new object[] { nombrenuevo });
                                }
                            }

                            else if (jugadornuevo > jugadorantiguo)
                            {

                                for (int i = jugadorantiguo; i < jugadornuevo; i++)
                                {
                                    string nombrenuevo = Convert.ToString(jugadores[i + 1]);
                                    this.Invoke(new DelegadoParaEscribir(EscribeTabla),
                                    new object[] { nombrenuevo });
                                }
                            }
                        }

                        else if (listacargada == false)
                        {
                            for (int i = 1; i < (Convert.ToInt32(jugadores[0]) + 1); i++)
                            {
                                string nombrenuevo = Convert.ToString(jugadores[i]);
                                this.Invoke(new DelegadoParaEscribir(EscribeTabla),
                                new object[] { nombrenuevo });
                                listacargada = true;
                            }
                        }
                        break;

                    case 6: //Invitacion
                        string peticion = Trozos[1];
                        string contrincante = Trozos[2];
                        Invitaciones(peticion, jugador, contrincante);
                        break;

                    case 7: //Desconectarse
                        server.Shutdown(SocketShutdown.Both);
                        server.Close();
                        int Jugadorantiguo = TablaUsuariosConectados.RowCount;
                        for (int i = 0; i < Jugadorantiguo; i++)
                        {
                            this.Invoke(new DelegadoParaNumerar(QuitaTabla),
                            new object[] { 0 });
                        }
                        this.Invoke(new DelegadoParaBoolear(PonButton4),
                        new object[] { false });
                        this.Invoke(new DelegadoParaBoolear(PonLabel1),
                        new object[] { false });
                        this.Invoke(new DelegadoParaBoolear(PonLabel2),
                        new object[] { false });
                        this.Invoke(new DelegadoParaBoolear(PonNombre),
                        new object[] { false });
                        this.Invoke(new DelegadoParaBoolear(PonContrasena),
                        new object[] { false });
                        this.Invoke(new DelegadoParaBoolear(PonButton4),
                        new object[] { false });
                        this.Invoke(new DelegadoParaBoolear(PonButton1),
                        new object[] { true });
                        this.Invoke(new DelegadoParaBoolear(PonTabla),
                        new object[] { false });
                        this.Invoke(new DelegadoParaBoolear(PonButton8),
                        new object[] { false });
                        this.Invoke(new DelegadoParaBoolear(PonButton9),
                        new object[] { false });
                        this.Invoke(new DelegadoParaBoolear(PonButton6),
                        new object[] { false });
                        this.Invoke(new DelegadoParaBoolear(PonGroupBox1),
                        new object[] { false });
                        this.Invoke(new DelegadoParaBoolear(PonLabel4),
                        new object[] { false });
                        this.Invoke(new DelegadoParaBoolear(PonButton3),
                        new object[] { false });
                        atendiendo = false;
                        break;
                    case 8: //Registrarse
                        string Usuario = Trozos[1];
                        string a = Trozos[2];
                        if (a == "SI")
                        {
                            MessageBox.Show(Usuario + " se ha registrado. Listo para apostar");
                        }
                        else
                        {
                            MessageBox.Show("Error al registrar");
                        }
                        break;
                    case 9: //Darse de baja
                        string Usuariodesconectado = Trozos[1];
                        string b = Trozos[2];
                        if (b == "SI")
                        {
                            MessageBox.Show(Usuariodesconectado + " se ha dado de baja. Ya no podrá apostar más aquí");
                        }
                        else
                        {
                            MessageBox.Show("Error al darse de baja");
                        }
                        break;
                    case 10: //Para actualizar las ganancias
                        int totalganado = Convert.ToInt32(Trozos[1]);
                        break;
                    case 13: //Para actualizar el numero que sale en la ruleta
                        Respuesta = Trozos[1];
                        ListaForm3[0].ActualizarNumero(Respuesta);
                        break;
                    case 14: //Actualiza el saldo segun la apuesta despues de que salga el numero
                        Respuesta = Trozos[1];
                        ListaForm3[0].ActualizarSaldo(Respuesta);
                        break;
                }
            }
        }

    }

}