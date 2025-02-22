/*------------------------------------------------------------------------------
* Copyright (C) 2003-2006 Ben van Klinken and the CLucene Team
* 
* Distributable under the terms of either the Apache License (Version 2.0) or 
* the GNU Lesser General Public License, as specified in the COPYING file.
------------------------------------------------------------------------------*/
#include "CLucene/StdHeader.h"
#include "StringBuffer.h"
#include "Misc.h"

CL_NS_DEF(util)

	StringBuffer::StringBuffer(TCHAR* buf,int32_t maxlen, const bool consumeBuffer){
		buffer = buf;
		bufferLength = maxlen;
		bufferOwner = !consumeBuffer;
	    len          = 0;
	}
  StringBuffer::StringBuffer(){
  //Func - Constructor. Allocates a buffer with the default length.
  //Pre  - true
  //Post - buffer of length bufferLength has been allocated
 
      //Initialize 
      bufferLength = LUCENE_DEFAULT_TOKEN_BUFFER_SIZE;
	  len          = 0;
	  //Allocate a buffer of length bufferLength
      buffer       = _CL_NEWARRAY(TCHAR,bufferLength);
	  bufferOwner  = true;
  }

  StringBuffer::StringBuffer(const int32_t initSize){
  //Func - Constructor. Allocates a buffer of length initSize + 1
  //Pre  - initSize > 0
  //Post - A buffer has been allocated of length initSize + 1

	  //Initialize the bufferLength to initSize + 1 The +1 is for the terminator '\0'
      bufferLength = initSize + 1;
      len = 0;
      //Allocate a buffer of length bufferLength
      buffer = _CL_NEWARRAY(TCHAR,bufferLength);
	  bufferOwner  = true;
  }

  StringBuffer::StringBuffer(const TCHAR* value){
  //Func - Constructor. 
  //       Creates an instance of Stringbuffer containing a copy of the string value
  //Pre  - value != NULL
  //Post - An instance of StringBuffer has been created containing the copy of the string value
  
      //Initialize the length of the string to be stored in buffer
	  len = (int32_t) _tcslen(value);

	  //Calculate the space occupied in buffer by a copy of value
      const int32_t occupiedLength = len + 1;
      
	  // Minimum allocated buffer length is LUCENE_DEFAULT_TOKEN_BUFFER_SIZE.
      bufferLength = (occupiedLength >= LUCENE_DEFAULT_TOKEN_BUFFER_SIZE 
	 	? occupiedLength : LUCENE_DEFAULT_TOKEN_BUFFER_SIZE);

	  //Allocate a buffer of length bufferLength
      buffer = _CL_NEWARRAY(TCHAR,bufferLength);
	  bufferOwner  = true;
      //Copy the string value into buffer
      _tcsncpy(buffer, value, occupiedLength);
	  //Assert that the buffer has been terminated at the end of the string
      CND_PRECONDITION (buffer[len] == '\0', "Buffer was not correctly terminated");
  }

  StringBuffer::~StringBuffer() {
  // Func - Destructor
  // Pre  - true
  // Post - Instanc has been destroyed

	if( bufferOwner ){
		_CLDELETE_CARRAY(buffer);
	}else
		buffer = NULL;
  }
  void StringBuffer::clear(){
  //Func - Clears the Stringbuffer and resets it to it default empty state
  //Pre  - true
  //Post - pre(buffer) has been destroyed and a new one has been allocated

      //Destroy the current buffer if present
	  _CLDELETE_CARRAY(buffer);

	  //Initialize 
      len = 0;
      bufferLength = LUCENE_DEFAULT_TOKEN_BUFFER_SIZE;
      //Allocate a buffer of length bufferLength
      buffer = _CL_NEWARRAY(TCHAR,bufferLength);
  }

  void StringBuffer::appendChar(const TCHAR character) {
  //Func - Appends a single character 
  //Pre  - true
  //Post - The character has been appended to the string in the buffer

	  //Check if the current buffer length is sufficient to have the string value appended
      if (len + 1 > bufferLength){
		   //Have the size of the current string buffer increased because it is too small	
          growBuffer(len + 1);
      }
	  //Put character at position len which is the end of the string in the buffer
	  //Note that this action might overwrite the terminator of the string '\0', which
	  //is kind of tricky
      buffer[len] = character;
	  //Increase the len by to represent the correct length of the string in the buffer
      len++;
  }

  void StringBuffer::append(const TCHAR* value) {
  //Func - Appends a copy of the string value
  //Pre  - value != NULL
  //Post - value has been copied and appended to the string in buffer
  
      append(value, _tcslen(value));
  }
  void StringBuffer::append(const TCHAR* value, size_t appendedLength) {
  //Func - Appends a copy of the string value
  //Pre  - value != NULL
  //       appendedLength contains the length of the string value which is to be appended
  //Post - value has been copied and appended to the string in buffer 
  
      //Check if the current buffer length is sufficient to have the string value appended
      if (len + appendedLength + 1 > bufferLength){
          //Have the size of the current string buffer increased because it is too small	
          growBuffer(len + appendedLength + 1);
      }

      //Copy the string value into the buffer at postion len
      _tcsncpy(buffer + len, value, appendedLength);
    
      //Add the length of the copied string to len to reflect the new length of the string in
      //the buffer (Note: len is not the bufferlength!)
      len += appendedLength;
  }

  void StringBuffer::appendInt(const int32_t value) {
  //Func - Appends an integer (after conversion to a character string)
  //Pre  - true 
  //Post - The converted integer value has been appended to the string in buffer
  
      //instantiate a buffer of 30 charactes for the conversion of the integer
      TCHAR buf[30];
      //Convert the integer value to a string buf using the radix 10 (duh)
      _i64tot(value, buf, 10);
	  //Have the converted integer now stored in buf appended to the string in buffer
      append(buf);
  }

  void StringBuffer::appendFloat(const float_t value, const int32_t digits){
  //Func - Appends a float_t (after conversion to a character string)
  //Pre  - digits > 0. Indicates the minimum number of characters printed
  //Post - The converted float_t value has been appended to the string in buffer

    //using sprintf("%f" was not reliable on other plaforms... we use a custom float convertor
    //bvk: also, using sprintf and %f seems excessivelly slow
	if(digits>8)
		_CLTHROWA(CL_ERR_IllegalArgument,"Too many digits...");

	  //the maximum number of characters that int64 will hold is 23. so we need 23*2+2
	 TCHAR buf[48]; //the buffer to hold
	 int64_t v = (int64_t)value; //the integer value of the float
	 _i64tot(v,buf,10); //add the whole number

	 size_t len = 99-_tcslen(buf); //how many digits we have to work with?
	 size_t dig = len< (size_t)digits ? len : digits;
	 if ( dig > 0 ){
		_tcscat(buf,_T(".")); //add a decimal point

		int64_t remi=(int64_t)((value-v)*pow((float_t)10,(float_t)(dig+1))); //take the remainder and make a whole number
		if ( remi<0 ) remi*=-1;
		int64_t remadj=remi/10;
		if ( remi-(remadj*10) >=5 )
			remadj++; //adjust remainder
		_i64tot(remadj,buf+_tcslen(buf),10); //add the remainder
	 }

	 append(buf);
  }

  void StringBuffer::prepend(const TCHAR* value){
  //Func - Puts a copy of the string value infront of the current string in the StringBuffer
  //Pre  - value != NULL
  //Post - The string in pre(buffer) has been shifted n positions where n equals the length of value.
  //       The string value was then copied to the beginning of stringbuffer

      prepend(value, _tcslen(value));
  }

  void StringBuffer::prepend(const TCHAR* value, const size_t prependedLength) {
  //Func - Puts a copy of the string value in front of the string in the StringBuffer
  //Pre  - value != NULL
  //       prependedLength contains the length of the string value which is to be prepended
  //Post - A copy of the string value is has been in front of the string in buffer
   //todo: something is wrong with this code, i'm sure... it only grows (and therefore moves if the buffer is to small)
	  //Check if the current buffer length is sufficient to have the string value prepended
	  if (prependedLength + len + 1 > bufferLength){
		  //Have the size of the current string buffer increased because it is too small	
		  //Because prependedLength is passed as the second argument to growBuffer,
          //growBuffer will have left the first prependedLength characters empty
          //when it recopied buffer during reallocation.  
          growBuffer(prependedLength + len + 1, prependedLength);
	  }

      //Copy the string value into the buffer at postion 0
      _tcsncpy(buffer, value, prependedLength);
      //Add the length of the copied string to len to reflect the new length of the string in
      //the buffer (Note: len is not the bufferlength!)
      len += prependedLength;
  }

  int32_t StringBuffer::length() const{
  //Func - Returns the length of the string in the StringBuffer
  //Pre  - true
  //Post - The length len of the string in the buffer has been returned

      return len;
  }
  TCHAR* StringBuffer::toString(){
  //Func - Returns a copy of the current string in the StringBuffer sized equal to the length of the string 
  //       in the StringBuffer.
  //Pre  - true
  //Post - The copied string has been returned

	  //Instantiate a buffer equal to the length len + 1
      TCHAR* ret = _CL_NEWARRAY(TCHAR,len + 1);
	  if (ret){
		  //Copy the string in buffer
	      _tcsncpy(ret, buffer, len);
		  //terminate the string
          ret[len] = '\0';
	  }
      //return the the copy  
      return ret;
  }
  TCHAR* StringBuffer::getBuffer() {
  //Func - '\0' terminates the buffer and returns its pointer
  //Pre  - true
  //Post - buffer has been '\0' terminated and returned
    
      // Check if the current buffer is '\0' terminated
	  if (len == bufferLength){
          //Make space for terminator, if necessary.
          growBuffer(len + 1);
	  }
      //'\0' buffer so it can be returned properly
      buffer[len] = '\0';

     return buffer;
  }
  
  void StringBuffer::reserve(const int32_t size){
  	if ( bufferLength >= size )
  		return;
	bufferLength = size;

	//Allocate a new buffer of length bufferLength
    TCHAR* tmp = _CL_NEWARRAY(TCHAR,bufferLength);
    _tcsncpy(tmp, buffer, len);
    tmp[len] = '\0';
	
	//destroy the old buffer
	if (buffer){
		_CLDELETE_CARRAY(buffer);
	}
	//Assign the new buffer tmp to buffer
    buffer = tmp;
  }

  void StringBuffer::growBuffer(const int32_t minLength) {
  //Func - Has the buffer grown to a minimum length of minLength or bigger
  //Pre  - minLength >= len + 1
  //Post - The buffer has been grown to a minimum length of minLength or bigger

      growBuffer(minLength, 0);
  }
  void StringBuffer::growBuffer(const int32_t minLength, const int32_t skippingNInitialChars) {
  //Func - Has the buffer grown to a minimum length of minLength or bigger and shifts the
  //       current string in buffer by skippingNInitialChars forward
  //Pre  - After growth, must have at least enough room for contents + terminator so
  //       minLength >= skippingNInitialChars + len + 1
  //       skippingNInitialChars >= 0
  //Post - The buffer has been grown to a minimum length of minLength or bigger and
  //       if skippingNInitialChars > 0, the contents of the buffer has beeen shifted
  //       forward by skippingNInitialChars positions as the buffer is reallocated,
  //       leaving the first skippingNInitialChars uninitialized (presumably to be
  //       filled immediately thereafter by the caller).

    CND_PRECONDITION (skippingNInitialChars >= 0, "skippingNInitialChars is less than zero");
    CND_PRECONDITION (minLength >= skippingNInitialChars + len + 1,"skippingNInitialChars is not large enough");

    //More aggressive growth strategy to offset smaller default buffer size:
	if ( !bufferOwner ){
		if ( bufferLength<minLength )
			_CLTHROWA(CL_ERR_IllegalArgument,"[StringBuffer::grow] un-owned buffer could not be grown");
		return;
	}
	
	bufferLength *= 2;
	//Check that bufferLength is bigger than minLength
	if (bufferLength < minLength){
	    //Have bufferLength become minLength because it still was too small
        bufferLength = minLength;
	}

	//Allocate a new buffer of length bufferLength
    TCHAR* tmp = _CL_NEWARRAY(TCHAR,bufferLength);
    //The old buffer might not have been null-terminated, so we _tcsncpy
    //only len bytes, not len+1 bytes (the latter might read one char off the
    //end of the old buffer), then apply the terminator to the new buffer.
    _tcsncpy(tmp + skippingNInitialChars, buffer, len);
    tmp[skippingNInitialChars + len] = '\0';
	
	//destroy the old buffer
	if (buffer){
		_CLDELETE_CARRAY(buffer);
	}
	//Assign the new buffer tmp to buffer
    buffer = tmp;
  }

CL_NS_END
