DROP DATABASE IF EXISTS M5;
CREATE DATABASE M5;
USE M5;
CREATE TABLE Jugador(
    IdJ INTEGER PRIMARY KEY NOT NULL,
    Nombre VARCHAR(20),
    Contrasena VARCHAR(20)
    )ENGINE = InnoDB;
CREATE TABLE Partida(
    IdP INTEGER PRIMARY KEY NOT NULL,
    Jugador1 VARCHAR(20),
    Jugador2 VARCHAR(20),
    Jugador3 VARCHAR(20),
    Fecha VARCHAR(20),
    Hora VARCHAR(20),
    Duracion INTEGER
    )ENGINE = InnoDB;
CREATE TABLE Historial(
    Jugador INTEGER NOT NULL,
    Partida INTEGER NOT NULL,
    Ganancias INTEGER,
    Perdidas INTEGER,
    FOREIGN KEY (Jugador) REFERENCES Jugador(IdJ),
    FOREIGN KEY (Partida) REFERENCES Partida(IdP)
    )ENGINE = InnoDB;

INSERT INTO Jugador VALUES (1, 'Reda', 'papa');
INSERT INTO Jugador VALUES (2, 'Joan', 'mama');
INSERT INTO Jugador VALUES (3, 'Marc', 'hermano');
INSERT INTO Partida VALUES (1, 'Reda', 'Joan', 'Marc', 'Lunes', '15:20', 20);
INSERT INTO Partida VALUES (2, 'Reda', 'Joan', 'Marc', 'Martes', '07:20', 50);
INSERT INTO Historial VALUES ('1', '2', '0', '896');
INSERT INTO Historial VALUES ('1', '1', '9', '0');
INSERT INTO Historial VALUES ('3', '2', '1987', '0');
INSERT INTO Historial VALUES ('3', '1', '24000', '0');       
