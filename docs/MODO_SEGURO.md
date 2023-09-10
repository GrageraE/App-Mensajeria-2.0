# Modo Seguro
Es una característica planeada para la versión 2.0. Sin embargo, se puede probar una versión preeliminar compilando este repositorio. El modo seguro incorporará dos funciones fundamentales:
1. Cifrado de datos por SSL
2. Protección de acceso al servidor por contraseña (opcional)

Se denegará el acceso a aquellos clientes que intenten conectarse sin soportar el modo seguro.
## Cifrado
El cifrado se realiza mediante SSL. Esta función protege la privacidad de los datos frente a ataques MITM. Para ello, se precisan de una `key` y de un certificado firmado. Para la conexión, el servidor deberá compartir, por medios seguros, dicho certificado al cliente. Los pasos para generar la `key` y un certificado autofirmado son los siguientes:
1. Generamos la `key`:
```
openssl genrsa 2048 > server.key
```
2. Generamos el certficado:
```
openssl req -new -x509 -nodes -sha256 -days 365 -key server.key -out server.cert
```
Dicho certificado no contendrá información del host (puede añadirse más adelante) y tendrá una vigencia de un año. Deberá de compartirse con los clientes por medios seguros.
## Protección por contraseña
El servidor puede establecer una contraseña de acceso. Se denegará el acceso a los clientes que no proporcionen la contraseña correcta, impidiendo el acceso no autorizado al chat.
