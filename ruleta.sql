DROP DATABASE IF EXISTS ruleta;
CREATE DATABASE ruleta;
USE ruleta;
CREATE TABLE Jugador(
    IdJ INTEGER PRIMARY KEY NOT NULL,
    Nombre TEXT,
    Contraseña TEXT
    )ENGINE = InnoDB;
CREATE TABLE Partida(
    IdP INTEGER PRIMARY KEY NOT NULL,
    Jugador1 TEXT,
    Jugador2 TEXT,
    Jugador3 TEXT,
    Fecha TEXT,
    Hora TEXT,
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
