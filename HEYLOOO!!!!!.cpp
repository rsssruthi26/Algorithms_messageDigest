

#include <string.h>
#include <iostream>
#include <fstream>
#include "sha256.h"
#include "BigIntegerLibrary.hh"

// Function which takes original message as input and set signature using D and N.
BigUnsigned signAndDecrypt(BigUnsigned originalMessage){


   // File function used here is fstream : https://stackoverflow.com/questions/16288562/how-is-stdfstream-with-both-in-and-out-supposed-to-work
   std::fstream d_n("d_n.txt", std::fstream::in | std::fstream::out);

   std::string swapValue;

   //Samelike how we swap ito the temporary value in sorting, here also we do the same to swap the lines in d_n.txt file using swwapValue.

   d_n >> swapValue;
   //convert the swapvalues from string to bigUnsigned, we have an inbuilt function stringToBigUnsigned that is first line(D to bigDecryptD and N to n)

   BigUnsigned bigDecryptD = stringToBigUnsigned(swapValue);
   std::cout << "D   " << bigDecryptD << "\n";////////////

   swapValue = "";
   d_n >> swapValue;


   BigUnsigned bigDecryptN = stringToBigUnsigned(swapValue);
   std::cout << "N  " << bigDecryptN << "\n";////////////

   //Sign/"decrypt" this hash value using the private key stored in d_n.txt, we use modexp function from th library to get the signature.
   
   BigUnsigned signature = modexp(originalMessage,bigDecryptD,bigDecryptN);

   return signature;

   

}

int main(int argc, char *argv[])
  {
    if((argc != 4 && argc != 3) || (argv[1][0]!='s' && argv[1][0]!='v'))
      std::cout<< "Program executed wrongly\n Collect format should be \"messageDigest435 s filename\"\n";
    else {
      std::string filename = argv[2];
      

      //read the file
      std::streampos begin,end;
      std::ifstream myfile (filename.c_str(), std::ios::binary);
      begin = myfile.tellg();
      myfile.seekg (0, std::ios::end);
      end = myfile.tellg();
      std::streampos size = end-begin;
      //std::cout << "size of the file: " << size << " bytes."; //size of the file

      myfile.seekg (0, std::ios::beg);
      char * memblock = new char[size];
      myfile.read (memblock, size); //read file; it's saved in the char array memblock
      myfile.close();

      std::string copyOFfile = filename+".Copy";
      std::ofstream myfile2 (copyOFfile.c_str(), std::ios::binary);
      myfile2.write (memblock, size); //write to a file
      myfile2.close();

      //std::cout<<memblock;


      if (argv[1][0]=='s')
      {
        std::cout << "Need to sign the doc.\n";

        //Step 1 : Generate a SHA‐256 hash of the content of the file to be signed

        std::string hashCode = sha256(memblock);
        
        //Step 2 : Convert the hashcode from base 16 to base 10

        BigUnsignedInABase hashCodeBase10 = BigUnsignedInABase(hashCode, 16);


        // Step 3 : Save the hashCodeBase10 to original message.

        BigUnsigned originalMessage = hashCodeBase10;

        std::cout << "Original Message " << originalMessage << "\n";////////////////

        //---------------------------------------------------------------------------//

        //Step 4 :   Sign/"decrypt" this hash value using the private key stored in d_n.txt
         BigUnsigned signSignature = signAndDecrypt(originalMessage);


        

        //Step 5 : Save the original content and the signature into one document filename.signed (e.g. "file.txt.signed")
        std::ofstream sign;
        sign.open( filename + ".signed");
        sign<< originalMessage << "\n"; ////////////////
        sign << signSignature;
        sign.close();
        std::cout << "Signature is done, use messageDigest435 v file.txt.signed for verification" ;

      }  

      // ..........Verify the signed file...........



      else if (argv[1][0] =='v') {


         std::cout << "Verification in process.";

        
        //Step 1 : Apply sha256 to memblock

        std::string verifySha256Hash = sha256(memblock);

        //Step 2 : convert verifySha256Hash from base 16 to base 10.

        BigUnsignedInABase VerifyShaBase16 = BigUnsignedInABase(verifySha256Hash, 16);

        //Step 3 : Store in orogianl message variable

        BigUnsigned originalMessage = VerifyShaBase16;
        
        // Step 4 : Get e and n from e_n.txt file
        std::fstream e_n("e_n.txt", std::fstream::in | std::fstream::out);

        //Step 5 : Use temp varaible for swapping the values of e and n and convert e and n to bigunsigned value

        std::string temp;
        e_n >> temp;
      
        BigUnsigned E = stringToBigUnsigned(temp); 
        temp = "";
        e_n >> temp;
        
        BigUnsigned N = stringToBigUnsigned(temp);
        e_n.close();


        //Step 6 : Separate the signature from the content of the file in the signed document (e.g. "file.txt.signed");

        std::fstream sign;
        std::string signatureKey;
        sign.open(argv[3]);


        // Step 7 : Get/read the signature from the file sign
        sign >> signatureKey;
        std::cout<< signatureKey;
        std::cout<< "signatureKey " << signatureKey << "\n";
        sign.close();



         // Step 8 : Convert this signature into BigUnsigned 

        BigUnsigned signature = stringToBigUnsigned(signatureKey);
        

         // Step 9 : Decrypt this signature with modexp function.

        BigUnsigned decryptedMessage = modexp(signature,E,N);
        

        
        if(originalMessage == decryptedMessage)
        {
          std::cout << "Document is authentic.";
        }
        else
        {
          std::cout << "Document is not authentic.\n";
        }   
      }
      delete[] memblock;
    }
  return 0;
  }
