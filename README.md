## General
____________

### Author
* Josh McIntyre

### Website
* jmcintyre.net

### Overview
* ProofOfWorker provides a hashcash-inspired proof of work implementation for web applications

## Development
________________

### Git Workflow
* master for releases (merge development)
* development for bugfixes and new features

### Building
* make build
Build the library
* make clean
Clean the build directory

### Features
* Ability to do proof-of-work in the browser for applications like anti-spam

### Requirements
* Requires Javascript

### Platforms
* Firefox
* Chrome

## Usage
____________

* Include the ProofOfWorker and CryptoJS libraries in the page
	`<script src="sha256.js" type="text/javascript"></script>`
	`<script src="proofofworker.js" type="text/javascript"></script>`
	
* Create a ProofOfWorker instance and call the `prove` function with the desired arguments
Pass in the desired hashing algorithm (Support: SHA-256)
Pass in the message
Pass in the difficulty exponent (2^difficulty exponent will be the difficulty target)
	`<script>`
		`pow = ProofOfWorker();`
		`nonce = pow.prove("SHA-256", "Hello", 240);`
	`</script>`