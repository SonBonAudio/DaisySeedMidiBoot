#ifndef __DynamicPatchDefinition_hpp__
#define __DynamicPatchDefinition_hpp__

#include "PatchDefinition.hpp"
#include "ProgramHeader.h"

class DynamicPatchDefinition : public PatchDefinition {
private:
  typedef void (*ProgramFunction)(void);
  ProgramFunction programFunction;
  uint32_t* linkAddress;
  uint32_t* jumpAddress;
  uint32_t* programAddress;
  uint32_t programSize;
  uint32_t binarySize;
  ProgramHeader* header;
  char programName[24];
public:
  DynamicPatchDefinition() :
    PatchDefinition(programName, AUDIO_CHANNELS, AUDIO_CHANNELS) {}
  DynamicPatchDefinition(void* addr, uint32_t sz) :
    PatchDefinition(programName, AUDIO_CHANNELS, AUDIO_CHANNELS) {
    load(addr, sz);
  }
  bool load(void* addr, uint32_t sz){
    binarySize = sz;
    programAddress = (uint32_t*)addr;
    header = (ProgramHeader*)addr;
    linkAddress = header->linkAddress;
    programSize = (uint32_t)header->endAddress - (uint32_t)header->linkAddress;
    if(sz != programSize)
      return false;
    stackBase = header->stackBegin;
    stackSize = (uint32_t)header->stackEnd - (uint32_t)header->stackBegin;
    jumpAddress = header->jumpAddress;
    programVector = header->programVector;
    strlcpy(programName, header->programName, sizeof(programName));
    programFunction = (ProgramFunction)jumpAddress;
    return true;
  }
  void copy(){
    /* copy program to ram */
    if (programAddress) {
      if(linkAddress != programAddress)
        memmove(linkAddress, programAddress, binarySize);
      programAddress = NULL;    
    }
  }
  bool verify(){
    // check we've got an entry function
    if(programFunction == NULL || programVector == NULL)
      return false;
    // check magic
    if((*(uint32_t*)programAddress & 0xffffff00) != 0xDADAC000) // was: != 0xDADAC0DE
      return false;
    extern char _PATCHRAM, _PATCHRAM_SIZE;
    if((linkAddress == (uint32_t*)&_PATCHRAM && programSize <= (uint32_t)(&_PATCHRAM_SIZE)))
      return true;
#ifdef USE_PLUS_RAM
    extern char _PLUSRAM, _PLUSRAM_SIZE;
    if((linkAddress == (uint32_t*)&_PLUSRAM && programSize <= (uint32_t)(&_PLUSRAM_SIZE)))
      return true;
#endif
    return false;
  }
  void run(){
    if(linkAddress != programAddress) {
      copy();
      if(binarySize < programSize) // blank out bss area
        memset(linkAddress+binarySize, 0, programSize - binarySize);
    }
    // We must verify that there's still a valid program function - it can be set to
    // NULL in copy method
    if (programFunction != NULL)
      programFunction();
  }
  uint32_t getProgramSize(){
    return programSize;
  }
  uint32_t* getLinkAddress(){
    return linkAddress;
  }
};


#endif // __DynamicPatchDefinition_hpp__
