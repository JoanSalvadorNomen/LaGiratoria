namespace WindowsFormsApplication1
{
    partial class Form1
    {
        /// <summary>
        /// Variable del diseñador requerida.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén utilizando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben eliminar; false en caso contrario, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido del método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.label2 = new System.Windows.Forms.Label();
            this.nombre = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.Partidas = new System.Windows.Forms.RadioButton();
            this.ganancias = new System.Windows.Forms.RadioButton();
            this.label3 = new System.Windows.Forms.Label();
            this.fechaBox = new System.Windows.Forms.TextBox();
            this.Registros = new System.Windows.Forms.RadioButton();
            this.button3 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.contrasena = new System.Windows.Forms.TextBox();
            this.button4 = new System.Windows.Forms.Button();
            this.TablaUsuariosConectados = new System.Windows.Forms.DataGridView();
            this.dataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.button5 = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.button6 = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.TablaUsuariosConectados)).BeginInit();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(39, 140);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(87, 25);
            this.label2.TabIndex = 1;
            this.label2.Text = "Nombre";
            // 
            // nombre
            // 
            this.nombre.Location = new System.Drawing.Point(169, 145);
            this.nombre.Name = "nombre";
            this.nombre.Size = new System.Drawing.Size(164, 20);
            this.nombre.TabIndex = 3;
            this.nombre.WordWrap = false;
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button1.Location = new System.Drawing.Point(302, 205);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(149, 31);
            this.button1.TabIndex = 4;
            this.button1.Text = "conectar";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(38, 158);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 5;
            this.button2.Text = "Enviar";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.groupBox1.Controls.Add(this.Partidas);
            this.groupBox1.Controls.Add(this.ganancias);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.fechaBox);
            this.groupBox1.Controls.Add(this.Registros);
            this.groupBox1.Controls.Add(this.button2);
            this.groupBox1.Location = new System.Drawing.Point(44, 81);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(328, 203);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Peticion";
            // 
            // Partidas
            // 
            this.Partidas.AutoSize = true;
            this.Partidas.Location = new System.Drawing.Point(31, 67);
            this.Partidas.Name = "Partidas";
            this.Partidas.Size = new System.Drawing.Size(176, 17);
            this.Partidas.TabIndex = 7;
            this.Partidas.TabStop = true;
            this.Partidas.Text = "Dime las partidas que he jugado";
            this.Partidas.UseVisualStyleBackColor = true;
            // 
            // ganancias
            // 
            this.ganancias.AutoSize = true;
            this.ganancias.Location = new System.Drawing.Point(31, 89);
            this.ganancias.Name = "ganancias";
            this.ganancias.Size = new System.Drawing.Size(170, 30);
            this.ganancias.TabIndex = 7;
            this.ganancias.TabStop = true;
            this.ganancias.Text = "Dime mis ganancias\r\n(Dia con inicial en mayusculas)";
            this.ganancias.UseVisualStyleBackColor = true;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(244, 84);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(37, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Fecha";
            // 
            // fechaBox
            // 
            this.fechaBox.Location = new System.Drawing.Point(247, 101);
            this.fechaBox.Name = "fechaBox";
            this.fechaBox.Size = new System.Drawing.Size(62, 20);
            this.fechaBox.TabIndex = 9;
            // 
            // Registros
            // 
            this.Registros.AutoSize = true;
            this.Registros.Location = new System.Drawing.Point(31, 44);
            this.Registros.Name = "Registros";
            this.Registros.Size = new System.Drawing.Size(168, 17);
            this.Registros.TabIndex = 8;
            this.Registros.TabStop = true;
            this.Registros.Text = "Dime los jugadores registrados";
            this.Registros.UseVisualStyleBackColor = true;
            // 
            // button3
            // 
            this.button3.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button3.Location = new System.Drawing.Point(304, 318);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(147, 35);
            this.button3.TabIndex = 10;
            this.button3.Text = "desconectar";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(39, 201);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(123, 25);
            this.label1.TabIndex = 11;
            this.label1.Text = "Contraseña";
            // 
            // contrasena
            // 
            this.contrasena.Location = new System.Drawing.Point(169, 205);
            this.contrasena.Name = "contrasena";
            this.contrasena.PasswordChar = '♠';
            this.contrasena.Size = new System.Drawing.Size(164, 20);
            this.contrasena.TabIndex = 12;
            this.contrasena.WordWrap = false;
            // 
            // button4
            // 
            this.button4.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button4.Location = new System.Drawing.Point(38, 54);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(149, 31);
            this.button4.TabIndex = 13;
            this.button4.Text = "Iniciar sesión";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // TablaUsuariosConectados
            // 
            this.TablaUsuariosConectados.AllowUserToAddRows = false;
            this.TablaUsuariosConectados.AllowUserToDeleteRows = false;
            this.TablaUsuariosConectados.AllowUserToOrderColumns = true;
            this.TablaUsuariosConectados.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.TablaUsuariosConectados.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.dataGridViewTextBoxColumn1});
            this.TablaUsuariosConectados.Location = new System.Drawing.Point(417, 106);
            this.TablaUsuariosConectados.Margin = new System.Windows.Forms.Padding(2);
            this.TablaUsuariosConectados.Name = "TablaUsuariosConectados";
            this.TablaUsuariosConectados.ReadOnly = true;
            this.TablaUsuariosConectados.RowHeadersWidth = 51;
            this.TablaUsuariosConectados.RowTemplate.Height = 24;
            this.TablaUsuariosConectados.RowTemplate.ReadOnly = true;
            this.TablaUsuariosConectados.Size = new System.Drawing.Size(285, 178);
            this.TablaUsuariosConectados.TabIndex = 14;
            this.TablaUsuariosConectados.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.TablaUsuariosConectados_CellContentClick);
            // 
            // dataGridViewTextBoxColumn1
            // 
            this.dataGridViewTextBoxColumn1.HeaderText = "Conectados";
            this.dataGridViewTextBoxColumn1.MinimumWidth = 6;
            this.dataGridViewTextBoxColumn1.Name = "dataGridViewTextBoxColumn1";
            this.dataGridViewTextBoxColumn1.ReadOnly = true;
            this.dataGridViewTextBoxColumn1.Width = 500;
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(468, 69);
            this.button5.Margin = new System.Windows.Forms.Padding(2);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(189, 34);
            this.button5.TabIndex = 15;
            this.button5.Text = "Actualizar lista de conectados";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(48, 47);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(0, 20);
            this.label4.TabIndex = 16;
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(489, 302);
            this.button6.Margin = new System.Windows.Forms.Padding(2);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(91, 28);
            this.button6.TabIndex = 17;
            this.button6.Text = "Invitar jugador";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::WindowsFormsApplication1.Properties.Resources.ruleta_casino_dados_195742_123;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(741, 389);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.TablaUsuariosConectados);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.contrasena);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.nombre);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.TablaUsuariosConectados)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox nombre;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton Partidas;
        private System.Windows.Forms.RadioButton Registros;
        private System.Windows.Forms.RadioButton ganancias;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox fechaBox;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox contrasena;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.DataGridView TablaUsuariosConectados;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn1;
        private System.Windows.Forms.Button button6;
    }
}

