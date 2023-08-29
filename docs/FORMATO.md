# Formato de envío de mensajes
## Definiciones
* `TIPO_STR`: Indica la razón del envío. Puede tomar los valores:
  * `MENSAJE_STR`: Es un mensaje cualquiera
  * `CONEXION_STR`: Mensaje de bienvenida, del cliente al servidor
  * `DESCONEXION_STR`: Mensaje de despedida (desconexión voluntaria) o denegación de acceso (si la manda el servidor)
  * `LISTA_STR`: Pide o envía la lista de usuarios conectados
* `USUARIO_STR`: Es el emisor del mensaje. Si es un string vacío (`""`) este mensaje es envíado por el servidor
* `CONTENIDO_STR`: Es el contenido del mensaje
  * Si el tipo es `LISTA_STR`, el contenido será la lista de usuarios
  * Si el tipo es `DESCONEXION_STR`, el contenido será la razón de denegación de acceso (nombre duplicado, expulsión, etc)
  * Si el tipo es `CONEXION_STR`, este campo estará vacío
## Ejemplos
#### Mensaje 
```
{
  TIPO_STR: MENSAJE_STR,
  USUARIO_STR: <usuario>,
  CONTENIDO_STR: <contenido>
}
```
#### Conexión
```
{
  TIPO_STR: CONEXION_STR,
  USUARIO_STR: <usuario>
}
```
#### Desconexión voluntaria
```
{
  TIPO_STR: DESCONEXION_STR,
  USUARIO_STR: <usuario>
}
```
#### Desconexión forzosa
```
{
  TIPO_STR: DESCONEXION_STR,
  USUARIO_STR: "",
  CONTENIDO_STR: <razon>
}
```
NOTA: Si aparece la razón `Servidor cerrado`, el servidor se ha cerrado. En este caso el mensaje se enviaría a todos los usuarios.
      Si aparece otra razón, el servidor ha denegado el acceso al usuario. Este mensaje solo se enviaría al usuario afectado, indicando la razón.
#### Petición de lista de usuarios
```
{
  TIPO_STR: LISTA_STR,
  USUARIO_STR: <usuario>
}
```
#### Entrega de lista de usuarios
```
{
  TIPO_STR: LISTA_STR,
  USUARIO_STR: "",
  CONTENIDO_STR: {
    "0": <usuario_1>,
    "1": <usuario_2>,
    ...
    }
}
```
NOTA: Este mensaje es privado: solo se envía al usuario que ha pedido la lista.
## Notas
1. Este documento cobrará valor con la versión 1.0. Hasta entonces es orientativo.
2. Como puede verse, en algunos casos `CONTENIDO_STR` puede ser un objeto JSON, siguiendo el formato mostrado.
3. Todos los mensajes se reenvían a todos los usuarios, excepto los privados.
