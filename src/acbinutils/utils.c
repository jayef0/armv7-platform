/* ex: set tabstop=2 expandtab: 
   -*- Mode: C; tab-width: 2; indent-tabs-mode nil -*-
*/
/**
 * @file      utils.c
 * @author    Alexandro Baldassin (UNICAMP)
 *            Daniel Casarotto (UFSC)
 *            Max Schultz (UFSC)
 * 
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 *            System Design Automation Lab (LAPS)
 *            INE-UFSC
 *            http://www.laps.inf.ufsc.br/
 * 
 * @version   1.0
 * @date      Thu, 01 Jun 2006 14:28:06 -0300
 * 
 * @brief     Utilities routines for the acbinutils module
 *            (implementation)
 * 
 * @attention Copyright (C) 2002-2006 --- The ArchC Team
 *
 */

#include <stdlib.h>
#include "utils.h"


static char *arch_name = NULL; /* name of the architecture */

/* assuming max input = 64 */
int log_table[] = {  0 /*invalid*/,  0, 1, 1, 
                     2 /* log 4 */,  2, 2, 2, 
                     3 /* log 8 */,  3, 3, 3,
                     3,              3, 3, 3,
                     4 /* log 16 */, 4, 4, 4 };

/* General internal error handling function */
void internal_error() {
  printf("Internal Error. Contact the ArchC Team.\n");
  exit(-1);
}

/*
 * Returns the format id of the string fname. (-1 if not found!)
 */
int get_format_id(char *fname)
{
  ac_dec_format *pfrm = format_ins_list;
  unsigned int formatid = 0;
  while ((pfrm != NULL) && strcmp(fname, pfrm->name)) {
    formatid++;
    pfrm = pfrm->next;
  }
  if (pfrm == NULL) return -1;
  return formatid;
}



/*
 * Returns the size of the architecture word (in bits)
 * It uses the ac_wordsize currently
 * Note that it only support architectures with, at most, 32-bit words
 */
unsigned int get_arch_size() 
{
  extern int wordsize;
  return wordsize ? wordsize : 32;
}

/*
 * Returns the size of an instruction format (in bits)
 */
unsigned int get_insn_size(ac_asm_insn *insn) 
{
  extern ac_dec_format *format_ins_list;
  ac_dec_format *pfrm = format_ins_list;
    
  if (insn->insn != NULL) { 

    while ((pfrm != NULL) && strcmp(insn->insn->format, pfrm->name)) 
        pfrm = pfrm->next;

    if (pfrm == NULL) internal_error();
    else return pfrm->size;
  }
  else internal_error();
}

void set_arch_name(char *str) 
{
  if (arch_name != NULL) {
    free(arch_name);
    arch_name = NULL;
  }

  if (str == NULL)
    return;
  
  arch_name = (char *) malloc(strlen(str)+1);
  strcpy(arch_name, str);
}

char *get_arch_name()
{
  return arch_name;
}

/*
 * Returns the max format size of the architecture word (in bits)
 */
unsigned int get_max_format_size() 
{
  extern ac_dec_format *format_ins_list;
  ac_dec_format *pfrm = format_ins_list;
  int max_size = 0;
             
  while (pfrm != NULL) {
    if (pfrm->size > max_size)
      max_size = pfrm->size;
    pfrm = pfrm->next;
  }

  return max_size;
}

/*
 * Returns if the architecture has variable formats (1) or no (0)
 */
unsigned int get_variable_format_size() 
{
  extern ac_dec_format *format_ins_list;
  ac_dec_format *pfrm = format_ins_list;

  int variable_format_size = 0;
  int size = pfrm->size;
               
  while (pfrm != NULL) {
    if (pfrm->size != size) {
      variable_format_size = 1;
      break;
    }
    pfrm = pfrm->next;
  }

  return variable_format_size;
}
