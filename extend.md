# Extend : 0.5.2
These are notes on how to extend the current revision of the soul file format standard. Anyone is freely allowed to give his/her opinions and notes.

## Current in-mind extensions

#### ***_DECLARE STATEMENT***
The **_DECLARE**, a similar statement or word can be used to refer to reusable variables. Currently we don't have support for reusable variables but soon this will be implemented. A similar thing can be done with groups. Anyone can provide contributions.

#### ***SET VARIABLE TEXT TO CONTENT IN OTHER FILE***
A Variable can be set to the content of an external file within the system using a sort of function identifier like '**FILE()**' provided by a system path to the file in parenthesis. This way files don't have to copied elsewhere or register a filepath in your program to get the file, instead you can import it from the soul document in a cross-platform way.

#### COLOR CONVERSIONS
Color conversions between HEX, RGB, CMYK, HSV, HSL easily from a soul file using a reader.

#### ENCRYPTING TEXT AND HASHING TEXT
##### ENCRYPTING
Maybe you will run upon critical or vulnerable data in your soul configuration files. In this scenario, you can either get the value, encrypt, decrypt it and store it temporarily in your program variable. But here, instead you can directly encrypt using some utility, store in file, then decrypt the value later using a soul reader and finally store the decrypted value in the soul map. What's useful here is, if you have another file with the same encrypted value, decrypt it using the soul reader and compare the decrypted value of both the text, making integrity and this all relies upons the key of the map and the key of the encryption.

##### HASHING
Hashing is a more better method of securing, suppose as the other user does not know the user content but only the hash.
This can be done using a soul reader method.

## END OF LINE FOR in-mind extensions...
