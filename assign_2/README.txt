
ACE 414 Assignment 2


Michail Marketakis 2017030165

To run the project you have to:


1) run "make" to make the executables "client" and "server".

2) Generate the Certificate by the following command :

"openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout mycert.pem -out mycert.pem "

3) in a terminal (A) run   sudo ./server port number    (sudo ./server 8082)

4) in a terminal (B) run ./client 127.0.0.1 port number (./client 127.0.0.1 8082)


Notes: I) You have to run 3) and 4) steps in diffrent terminals!
       II)You have to use the same port number.
       
 

Then client has to give his name and his password.
if( name ==Sousi and password=123) then the response by the server will be:

<Body>
<Name>sousi.com</Name>
<year>1.5</year>
<BlogType>Embedede and c c++</BlogType>
<Author>John Johny</Author>
<Body>


otherwise the response by the server will be:

"Invalid Message"

