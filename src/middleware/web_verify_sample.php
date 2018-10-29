<?php

/* This file contains code that collects a proof of work validation request from
* some client-side application via GET, formats the request, and sends it to 
* the validation server over a socket.
* Replace the server and port information with your own, and add some actions
* to do when proof of work is valid or invalid
*
* Author: Josh McIntyre
*/

// Generate the verification request
$message = $_GET["message"];
$nonce = $_GET["nonce"];
$exponent = $_GET["exponent"];
$algorithm = $_GET["algorithm"];

if (empty($message) || empty($nonce) || empty($exponent) || empty($algorithm))
{
	die("Missing one or more required GET parameters: message, nonce, exponent, algorithm.");
}

$ver_request = $message.":".$nonce.":".$exponent.":".$algorithm;

// Create the socket and connect
$pow_server = "127.0.0.1";
$pow_server_port = 8001;

$socket = socket_create(AF_INET, SOCK_STREAM, 0) or die("Unable to create socket for verification server");
$conn_result = socket_connect($socket, $pow_server, $pow_server_port) or die ("Unable to connect to verification server socket");

// Write the verification request to the socket
socket_write($socket, $ver_request, strlen($ver_request)) or die ("Unable to send verification request to server");

// Read the response
$ver_response = socket_read($socket, 5);

/* Do the approved action if proof of work is valid
* Otherwise do some failure action
*/
if (strstr($ver_response, "true") !== false)
{
	echo "Proof of work verified!";
}
else
{
	echo "Proof of work invalid";
}

socket_close($socket);

?>
