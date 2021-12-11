/*
Class Project: The logical conclusion (v1.1)
CSCI-2500 Fall 2021
This program simulates a full datapath, imitating
the progression of instructions through the 5
stages of what is a single-cycle architecture here.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

// useful constants
BIT ONE[32] = { TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
               FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
               FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
               FALSE };
BIT ZERO[32] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                FALSE };
BIT REG_THIRTY_ONE[5] = { TRUE, TRUE, TRUE, TRUE, TRUE };
BIT THIRTY_TWO[32] = { FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE,
                      FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                      FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                      FALSE };

/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT or_gate3(BIT A, BIT B, BIT C);
BIT and_gate(BIT A, BIT B);
BIT and_gate3(BIT A, BIT B, BIT C);
BIT and_gate5(BIT A, BIT B, BIT C, BIT D, BIT E);
BIT and_gate6(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);
BIT nand_gate(BIT A, BIT B);

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3);
void decoder3(BIT* I, BIT EN, BIT* O);
void decoder5(BIT* I, BIT* O);
BIT multiplexor2(BIT S, BIT I0, BIT I1);
void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output);
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);

void copy_bits(BIT* A, BIT* B);
void print_binary(BIT* A);
void convert_to_binary(int a, BIT* A, int length);
int binary_to_integer(BIT* A);

int get_instructions(BIT Instructions[][32]);

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction);
void Control(BIT* OpCode, BIT* funct,
  BIT* RegDst0, BIT* RegDst1, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg0,
  BIT* MemToReg1, BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite, BIT* JumpReg);
void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2);
void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData);
void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl);
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result);
void Data_Memory(BIT MemWrite, BIT MemRead,
  BIT* Address, BIT* WriteData, BIT* ReadData);
void Extend_Sign16(BIT* Input, BIT* Output);
void updateState();

/******************************************************************************/
/* Functions provided for your convenience */
/******************************************************************************/
BIT not_gate(BIT A)
{
  if (A)
    return FALSE;
  else
    return TRUE;
}

BIT or_gate(BIT A, BIT B)
{
  if (A || B)
    return TRUE;
  else
    return FALSE;
}
BIT or_gate3(BIT A, BIT B, BIT C)
{
  return or_gate(A, or_gate(B, C));
}
// Extended OR Gates
BIT or_gate4(BIT A, BIT B, BIT C, BIT D)
{
  return or_gate(or_gate(A, B), or_gate(C, D));
}
BIT or_gate5(BIT A, BIT B, BIT C, BIT D, BIT E)
{
  return or_gate(or_gate(or_gate(A, B), or_gate(C, D)), E);
}
BIT or_gate6(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F)
{
  return or_gate(or_gate(or_gate(A, B), or_gate(C, D)), or_gate(E, F));
}
BIT or_gate7(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F, BIT G)
{
  return or_gate(or_gate(or_gate(or_gate(A, B), or_gate(C, D)), or_gate(E, F)), G);
}
BIT or_gate8(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F, BIT G, BIT H)
{
  return or_gate(or_gate(or_gate(or_gate(A, B), or_gate(C, D)),
    or_gate(E, F)), or_gate(G, H));
}
BIT or_gate32(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F, BIT G,
  BIT H, BIT I, BIT J, BIT K, BIT L, BIT M, BIT N, BIT O, BIT P,
  BIT Q, BIT R, BIT S, BIT T, BIT U, BIT V, BIT W, BIT X, BIT Y,
  BIT Z, BIT AA, BIT BB, BIT CC, BIT DD, BIT EE, BIT FF) {
  return or_gate8(or_gate4(A, B, C, D), or_gate4(E, F, G, H),
    or_gate4(I, J, K, L), or_gate4(M, N, O, P), or_gate4(Q, R, S, T),
    or_gate4(U, V, W, X), or_gate4(Y, Z, AA, BB), or_gate4(CC, DD, EE, FF));
}

BIT and_gate(BIT A, BIT B)
{
  if (A && B)
    return TRUE;
  else
    return FALSE;
}

BIT and_gate3(BIT A, BIT B, BIT C)
{
  return and_gate(A, and_gate(B, C));
}
// Extended AND gates
BIT and_gate4(BIT A, BIT B, BIT C, BIT D)
{
  return and_gate(A, and_gate(B, and_gate(C, D)));
}
BIT and_gate5(BIT A, BIT B, BIT C, BIT D, BIT E)
{
  return and_gate(and_gate(and_gate(A, B), and_gate(C, D)), E);
}
BIT and_gate6(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F)
{
  return and_gate(and_gate(and_gate(A, B), and_gate(C, D)), and_gate(E, F));
}
BIT and_gate7(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F, BIT G)
{
  return and_gate(and_gate(and_gate(and_gate(A, B), and_gate(C, D)), and_gate(E, F)), G);
}
BIT and_gate8(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F, BIT G, BIT H)
{
  return and_gate(and_gate(and_gate(and_gate(A, B), and_gate(C, D)),
    and_gate(E, F)), and_gate(G, H));
}
BIT and_gate12(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F, BIT G, BIT H, BIT I, BIT J, BIT K, BIT L)
{
  return and_gate(and_gate6(A, B, C, D, E, F), and_gate6(G, H, I, J, K, L));
}
BIT and_gate32(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F, BIT G,
  BIT H, BIT I, BIT J, BIT K, BIT L, BIT M, BIT N, BIT O, BIT P,
  BIT Q, BIT R, BIT S, BIT T, BIT U, BIT V, BIT W, BIT X, BIT Y,
  BIT Z, BIT AA, BIT BB, BIT CC, BIT DD, BIT EE, BIT FF) {
  return and_gate8(and_gate4(A, B, C, D), and_gate4(E, F, G, H),
    and_gate4(I, J, K, L), and_gate4(M, N, O, P), and_gate4(Q, R, S, T),
    and_gate4(U, V, W, X), and_gate4(Y, Z, AA, BB), and_gate4(CC, DD, EE, FF));
}


BIT xor_gate(BIT A, BIT B)
{
  if (A ^ B)
    return TRUE;
  else
    return FALSE;
}

BIT nor_gate(BIT A, BIT B)
{
  return not_gate(or_gate(A, B));
}

BIT nand_gate(BIT A, BIT B)
{
  return not_gate(and_gate(A, B));
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  // 2-input decoder implemented in labs 5 & 6
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(not_gate(I1), I0);
  *O2 = and_gate(I1, not_gate(I0));
  *O3 = and_gate(I1, I0);

  return;
}
void decoder3(BIT* I, BIT EN, BIT* O)
{
  // 3-input decoder implemented in labs 5 & 6
  O[0] = and_gate3(not_gate(I[2]), not_gate(I[1]), not_gate(I[0]));
  O[1] = and_gate3(not_gate(I[2]), not_gate(I[1]), I[0]);
  O[2] = and_gate3(not_gate(I[2]), I[1], not_gate(I[0]));
  O[3] = and_gate3(not_gate(I[2]), I[1], I[0]);
  O[4] = and_gate3(I[2], not_gate(I[1]), not_gate(I[0]));
  O[5] = and_gate3(I[2], not_gate(I[1]), I[0]);
  O[6] = and_gate3(I[2], I[1], not_gate(I[0]));
  O[7] = and_gate3(I[2], I[1], I[0]);

  O[0] = and_gate(EN, O[0]);
  O[1] = and_gate(EN, O[1]);
  O[2] = and_gate(EN, O[2]);
  O[3] = and_gate(EN, O[3]);
  O[4] = and_gate(EN, O[4]);
  O[5] = and_gate(EN, O[5]);
  O[6] = and_gate(EN, O[6]);
  O[7] = and_gate(EN, O[7]);

  return;
}

void decoder5(BIT* I, BIT* O)
{
  /*
    -- 5-input decoder implemented in labs 5 & 6
    -- Note the use of a 2-decoder to set enabler bits
    -- 3-decoders used to set output bits in 8-step increments
  */

  BIT EN[4] = { FALSE };
  decoder2(I[3], I[4], &EN[0], &EN[1], &EN[2], &EN[3]);
  decoder3(I, EN[3], &O[24]);
  decoder3(I, EN[2], &O[16]);
  decoder3(I, EN[1], &O[8]);
  decoder3(I, EN[0], &O[0]);
}



BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  // 1-input multiplexor implemented in labs 5 & 6
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);
}

void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output)
{
  // 1-input multiplexor utility on all 32 bits of two inputs
  for (int i = 0; i < 32; ++i)
  {
    BIT x0 = and_gate(not_gate(S), I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  // 2-input multiplexor implemented in labs 5 & 6
  BIT x0, x1, x2, x3 = FALSE;
  decoder2(S0, S1, &x0, &x1, &x2, &x3);

  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);

  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);

  return or_gate(z0, z1);
}
void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
  // 1-bit adder implemented in lab 5
  BIT x0 = xor_gate(A, B);
  *Sum = xor_gate(CarryIn, x0);

  BIT y0 = and_gate(x0, CarryIn);
  BIT y1 = and_gate(A, B);
  *CarryOut = or_gate(y0, y1);
}

void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less,
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set)
{
  // 1-bit ALU as implemented in lab 6
  BIT x0 = multiplexor2(Binvert, B, not_gate(B));

  BIT y0 = and_gate(A, x0);
  BIT y1 = or_gate(A, x0);

  BIT y2 = FALSE;
  adder1(A, x0, CarryIn, CarryOut, &y2);
  *Set = y2;

  BIT y3 = Less;

  *Result = multiplexor4(Op0, Op1, y0, y1, y2, y3);
}

/******************************************************************************/
/* Helper functions */
/******************************************************************************/
void copy_bits(BIT* A, BIT* B)
{
  for (int i = 0; i < 32; ++i)
    B[i] = A[i];
}

void print_binary(BIT* A)
{
  for (int i = 31; i >= 0; --i)
    printf("%d", A[i]);
}

void convert_to_binary(int a, BIT* A, int length)
{
  // Utility to convert offset values provided in instruction 
  memset(A, '\0', length);
  // Set all bits to 0
  for (int i = 0; i < length; i++)
  {
    // Set A[i] to LSB in the input
    A[i] = a & 1;
    // Shift input right by 1
    a = a >> 1;
  }
}

int binary_to_integer(BIT* A)
{
  /*
     -- Debugging utility to convert binary
        array to an integer value
  */
  unsigned a = 0;
  unsigned mult = 1;

  for (int i = 0; i < 32; ++i)
  {
    a += A[i] * mult;
    mult *= 2;
  }

  return (int)a;
}

/******************************************************************************/
/* Parsing functions */
/******************************************************************************/

// TODO: Implement any helper functions to assist with parsing
int get_opcode(char* instruction)
{
  /* This function retrieves the integer opcode from the
     instruction string using string comparisons.
  */
  if (strcmp(instruction, "lw") == 0)
  {
    return 35;
  }
  else if (strcmp(instruction, "sw") == 0)
  {
    return 43;
  }
  else if (strcmp(instruction, "addi") == 0)
  {
    return 8;
  }
  else if (strcmp(instruction, "beq") == 0)
  {
    return 4;
  }
  else if (strcmp(instruction, "j") == 0)
  {
    return 2;
  }
  else if (strcmp(instruction, "jal") == 0)
  {
    return 3;
  }
  {
    /*
      -- R-type instructions and jr have opcode = 0
         with specific funct values.
    */

    return 0;
  }
}
int get_register_number(char* reg)
{
  /* This function retrieves the register number associated
     with a register name using string comparisons. Note
     that we only account for those registers enumerated
     below.
  */
  if (strcmp(reg, "zero") == 0)
  {
    return 0;
  }
  else if (strcmp(reg, "v0") == 0)
  {
    return 2;
  }
  else if (strcmp(reg, "t0") == 0)
  {
    return 8;
  }
  else if (strcmp(reg, "t1") == 0)
  {
    return 9;
  }
  else if (strcmp(reg, "a0") == 0)
  {
    return 4;
  }
  else if (strcmp(reg, "s0") == 0)
  {
    return 16;
  }
  else if (strcmp(reg, "s1") == 0)
  {
    return 17;
  }
  else if (strcmp(reg, "sp") == 0)
  {
    return 29;
  }
  else if (strcmp(reg, "ra") == 0)
  {
    return 31;
  }
  else
  {
    // Undefined register
    return -1;
  }
}
int get_funct(char* instruction)
{
  //IMPLEMENTED BY: TYLER
  if (strcmp(instruction, "add") == 0)
  {
    return 32;
  }
  else if (strcmp(instruction, "sub") == 0)
  {
    return 34;
  }
  else if (strcmp(instruction, "and") == 0)
  {
    return 36;
  }
  else if (strcmp(instruction, "or") == 0)
  {
    return 37;
  }
  else if (strcmp(instruction, "slt") == 0)
  {
    return 42;
  }
  else if (strcmp(instruction, "jr") == 0)
  {
    return 8;
  }
  else
  {
    return -1;
  }
}

int get_instructions(BIT Instructions[][32])
{
  /* This function reads assembly language in MIPS
     from stdin and converts that instruction to a
     binary format for later fetching.
  */
  //READ INSTRUCTIONS FROM STDIN
  char line[256] = { 0 };
  int instruction_count = 0;
  while (fgets(line, 256, stdin) != NULL)
  {
    //Initialize string components of instruction
    /*
      -- Note that we assume a consistent format to the
         instruction here.
    */

    char instruction[16];
    char dest[16];
    char src1[16];
    char src2[16];
    memset(instruction, '\0', 16);
    memset(dest, '\0', 16);
    memset(src1, '\0', 16);
    memset(src2, '\0', 16);
    //Read components of instruction
    sscanf(line, "%s %s %s %s", instruction, dest, src1, src2);
    //Determine behavior of instruction
    //Convert each component to binary format
    if (strcmp(instruction, "lw") == 0 || strcmp(instruction, "sw") == 0 ||
      strcmp(instruction, "addi") == 0)
    {
      // I-type instructions

      // Immediate - Assume an integer value here
      char* END = NULL;
      // Convert string to integer
      int immediate = strtol(src2, &END, 10);
      BIT* immediate_bits = malloc(16);
      convert_to_binary(immediate, immediate_bits, 16);
      // Reg2 - source 1 (assume a register name here)
      // Get register number
      int reg2 = get_register_number(src1);
      BIT* reg2_bits = malloc(5);
      //Convert to binary
      convert_to_binary(reg2, reg2_bits, 5);
      // Reg1 - Destination (assume a register name here)
      int reg1 = get_register_number(dest);
      BIT* reg1_bits = malloc(5);
      convert_to_binary(reg1, reg1_bits, 5);
      // Opcode from instruction name
      int opcode = get_opcode(instruction);
      BIT* opcode_bits = malloc(6);
      convert_to_binary(opcode, opcode_bits, 6);
      /*
        -- Set instruction bits directly in instruction memory
        -- Assign address correlating to line number effectively
      */
      // Immediate
      for (int i = 0; i < 16; i++)
      {
        Instructions[instruction_count][i] = immediate_bits[i];
      }
      // Destination register
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 16] = reg1_bits[i];
      }
      // Source register
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 21] = reg2_bits[i];
      }
      // Opcode
      for (int i = 0; i < 6; i++)
      {
        Instructions[instruction_count][i + 26] = opcode_bits[i];
      }
    }
    else if (strcmp(instruction, "beq") == 0) {
      // Special behavior for beq
      char* END = NULL;
      int immediate = strtol(src2, &END, 10);
      BIT* immediate_bits = malloc(16);
      convert_to_binary(immediate, immediate_bits, 16);
      //Reg2 - source 1
      int reg2 = get_register_number(src1);
      BIT* reg2_bits = malloc(5);
      convert_to_binary(reg2, reg2_bits, 5);
      //Reg1 - Destination
      int reg1 = get_register_number(dest);
      BIT* reg1_bits = malloc(5);
      convert_to_binary(reg1, reg1_bits, 5);
      //Opcode
      int opcode = get_opcode(instruction);
      BIT* opcode_bits = malloc(6);
      convert_to_binary(opcode, opcode_bits, 6);
      // Set bits in instruction memory
      //Immediate branch offset
      for (int i = 0; i < 16; i++)
      {
        Instructions[instruction_count][i] = immediate_bits[i];
      }
      //Second source register
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 16] = reg2_bits[i];
      }
      // First source register
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 21] = reg1_bits[i];
      }
      // Opcode
      for (int i = 0; i < 6; i++)
      {
        Instructions[instruction_count][i + 26] = opcode_bits[i];
      }
    }
    else if (strcmp(instruction, "j") == 0 || strcmp(instruction, "jal") == 0)
    {
      //J-type instructions
      // Immediate
      char* END = NULL;
      int immediate = strtol(dest, &END, 10);
      // 26-bits for the immediate value
      BIT* immediate_bits = malloc(26);
      convert_to_binary(immediate, immediate_bits, 26);
      //Opcode
      int opcode = get_opcode(instruction);
      BIT* opcode_bits = malloc(6);
      convert_to_binary(opcode, opcode_bits, 6);
      // Immediate jump address
      for (int i = 0; i < 26; i++)
      {
        Instructions[instruction_count][i] = immediate_bits[i];
      }
      // Opcode
      for (int i = 0; i < 6; i++)
      {
        Instructions[instruction_count][i + 26] = opcode_bits[i];
      }
    }
    else if (strcmp(instruction, "jr") == 0)
    {
      //Handle jr instruction separately
      //Reg1 - Source 1
      int reg1 = get_register_number(dest);
      BIT* reg1_bits = malloc(5);
      convert_to_binary(reg1, reg1_bits, 5);
      //Opcode
      int funct = get_funct(instruction);
      BIT* funct_bits = malloc(6);
      convert_to_binary(funct, funct_bits, 6);
      memset(Instructions[instruction_count], '\0', 32);
      // Funct bits
      for (int i = 0; i < 6; i++)
      {
        Instructions[instruction_count][i] = funct_bits[i];
      }
      // Register that contains the address to which we are returning
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 21] = reg1_bits[i];
      }
    }
    else
    {
      //R-type instructions

      //Opcode
      BIT* opcode_bits = malloc(6);
      memset(opcode_bits, '\0', 6);
      //Reg3 - rs1
      int reg3 = get_register_number(src2);
      BIT* reg3_bits = malloc(5);
      convert_to_binary(reg3, reg3_bits, 5);
      //Reg2 - rs2
      int reg2 = get_register_number(src1);
      BIT* reg2_bits = malloc(5);
      convert_to_binary(reg2, reg2_bits, 5);
      //Reg1 - rd
      int reg1 = get_register_number(dest);
      BIT* reg1_bits = malloc(5);
      convert_to_binary(reg1, reg1_bits, 5);
      //Shamt - Don't account for sll or srl so always 0
      BIT* shamt_bits = malloc(5);
      memset(shamt_bits, '\0', 5);
      //Funct;
      int funct = get_funct(instruction);
      BIT* funct_bits = malloc(6);
      convert_to_binary(funct, funct_bits, 6);
      // Funct
      for (int i = 0; i < 6; i++)
      {
        Instructions[instruction_count][i] = funct_bits[i];
      }
      // Shamt - unused 
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 6] = shamt_bits[i];
      }
      // Destination register
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 11] = reg1_bits[i];
      }
      // Source register 1
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 16] = reg3_bits[i];
      }
      // Source Register 2
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 21] = reg2_bits[i];
      }
      for (int i = 0; i < 6; i++)
      {
        Instructions[instruction_count][i + 26] = opcode_bits[i];
      }
    }
    instruction_count++;
  }
  return instruction_count;
}

/******************************************************************************/
/* Program state - memory spaces, PC, and control */
/******************************************************************************/
BIT PC[32] = { FALSE };
BIT MEM_Instruction[32][32] = { FALSE };
BIT MEM_Data[32][32] = { FALSE };
BIT MEM_Register[32][32] = { FALSE };

BIT RegDst0 = FALSE;
BIT RegDst1 = FALSE;
BIT Jump = FALSE;
BIT Branch = FALSE;
BIT MemRead = FALSE;
BIT MemToReg0 = FALSE;
BIT MemToReg1 = FALSE;
BIT ALUOp[2] = { FALSE };
BIT MemWrite = FALSE;
BIT ALUSrc = FALSE;
BIT RegWrite = FALSE;
BIT Zero = FALSE;
BIT JumpReg = FALSE;
BIT ALUControl[4] = { FALSE };

void print_control_bits() {
  printf("RegDst0: %d\n", RegDst0);
  printf("RegDst1: %d\n", RegDst1);
  printf("Jump: %d\n", Jump);
  printf("Branch: %d\n", Branch);
  printf("MemRead: %d\n", MemRead);
  printf("MemToReg0: %d\n", MemToReg0);
  printf("MemToReg1: %d\n", MemToReg1);
  printf("ALUOp[0]: %d\n", ALUOp[0]);
  printf("ALUOp[1]: %d\n", ALUOp[1]);
  printf("MemWrite: %d\n", MemWrite);
  printf("ALUSrc: %d\n", ALUSrc);
  printf("RegWrite: %d\n", RegWrite);
  printf("Zero: %d\n", Zero);
  printf("ALUControl[0]: %d\n", ALUControl[0]);
  printf("ALUControl[1]: %d\n", ALUControl[1]);
  printf("ALUControl[2]: %d\n", ALUControl[2]);
  printf("ALUControl[3]: %d\n", ALUControl[3]);
}
void print_instruction()
{
  unsigned pc = binary_to_integer(PC);
  printf("PC: %d\n", pc);
  printf("Instruction: ");
  print_binary(MEM_Instruction[pc]);
  printf("\n");
}

void print_state()
{
  printf("Data: ");
  for (int i = 0; i < 32; ++i)
  {
    printf("%d ", binary_to_integer(MEM_Data[i]));
  }
  printf("\n");

  printf("Register: ");
  for (int i = 0; i < 32; ++i)
  {
    printf("%d ", binary_to_integer(MEM_Register[i]));
  }
  printf("\n");
}

/******************************************************************************/
/* Functions that you will implement */
/******************************************************************************/
void Instruction_Memory(BIT* ReadAddress, BIT* Instruction)
{
  /* This function retrieves the instruction at the given
     address in a binary format. */
     // Input: 32-bit instruction address
     // Output: 32-bit binary instruction
  BIT* O = malloc(32);
  // Use 5-decoder to identify address we want
  decoder5(ReadAddress, O);
  // From all 32 register in I-mem, only choose the bits from the address we want
  for (int i = 0;i < 32;i++) {
    /* Note that this will evaluate to one only if there is a one
      at the i^th bit of the instruction stored at the given address. */
    Instruction[i] = or_gate32(and_gate(O[0], MEM_Instruction[0][i]), and_gate(O[1], MEM_Instruction[1][i]),
      and_gate(O[2], MEM_Instruction[2][i]), and_gate(O[3], MEM_Instruction[3][i]),
      and_gate(O[4], MEM_Instruction[4][i]), and_gate(O[5], MEM_Instruction[5][i]),
      and_gate(O[6], MEM_Instruction[6][i]), and_gate(O[7], MEM_Instruction[7][i]),
      and_gate(O[8], MEM_Instruction[8][i]), and_gate(O[9], MEM_Instruction[9][i]),
      and_gate(O[10], MEM_Instruction[10][i]), and_gate(O[11], MEM_Instruction[11][i]),
      and_gate(O[12], MEM_Instruction[12][i]), and_gate(O[13], MEM_Instruction[13][i]),
      and_gate(O[14], MEM_Instruction[14][i]), and_gate(O[15], MEM_Instruction[15][i]),
      and_gate(O[16], MEM_Instruction[16][i]), and_gate(O[17], MEM_Instruction[17][i]),
      and_gate(O[18], MEM_Instruction[18][i]), and_gate(O[19], MEM_Instruction[19][i]),
      and_gate(O[20], MEM_Instruction[20][i]), and_gate(O[21], MEM_Instruction[21][i]),
      and_gate(O[22], MEM_Instruction[22][i]), and_gate(O[23], MEM_Instruction[23][i]),
      and_gate(O[24], MEM_Instruction[24][i]), and_gate(O[25], MEM_Instruction[25][i]),
      and_gate(O[26], MEM_Instruction[26][i]), and_gate(O[27], MEM_Instruction[27][i]),
      and_gate(O[28], MEM_Instruction[28][i]), and_gate(O[29], MEM_Instruction[29][i]),
      and_gate(O[30], MEM_Instruction[30][i]), and_gate(O[31], MEM_Instruction[31][i]));
  };

}

void Control(BIT* OpCode, BIT* funct,
  BIT* RegDst0, BIT* RegDst1, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg0,
  BIT* MemToReg1, BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite, BIT* JumpReg)
{
  /* This function sets all control bits in the simulated datapath.*/
  /*
    -- SOP to determine control bits
    -- Introduced four new bits:
      -- JumpReg for jr
      -- RegDst[1] for jal
      -- Jump for j
      -- MemToReg[1] for jal as well
      -- See table in writeup for development of SOP's
  */
  * RegDst1 = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]);
  *RegDst0 = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), not_gate(OpCode[1]),
    not_gate(OpCode[0]));
  *ALUSrc = and_gate6(OpCode[5], not_gate(OpCode[4]), not_gate(OpCode[3]),
    not_gate(OpCode[2]), OpCode[1], OpCode[0]) ||
    and_gate6(OpCode[5], not_gate(OpCode[4]), OpCode[3],
      not_gate(OpCode[2]), OpCode[1], OpCode[0]) ||
    and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), OpCode[3],
      not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0]));
  *MemToReg1 = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]);
  *MemToReg0 = and_gate6(OpCode[5], not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]);
  *RegWrite = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), not_gate(OpCode[1]),
    not_gate(OpCode[0])) ||
    and_gate6(OpCode[5], not_gate(OpCode[4]), not_gate(OpCode[3]),
      not_gate(OpCode[2]), OpCode[1], OpCode[0]) ||
    and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
      not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]) ||
    and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), OpCode[3],
      not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0]));
  *MemRead = and_gate6(OpCode[5], not_gate(OpCode[4]), not_gate(OpCode[3]),
    not_gate(OpCode[2]), OpCode[1], OpCode[0]);
  *MemWrite = and_gate6(OpCode[5], not_gate(OpCode[4]), OpCode[3],
    not_gate(OpCode[2]), OpCode[1], OpCode[0]);
  *Branch = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), OpCode[2], not_gate(OpCode[1]),
    not_gate(OpCode[0]));
  ALUOp[1] = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), not_gate(OpCode[1]),
    not_gate(OpCode[0])) || and_gate6(not_gate(OpCode[5]),
      not_gate(OpCode[4]), OpCode[3], not_gate(OpCode[2]),
      not_gate(OpCode[1]), not_gate(OpCode[0]));
  ALUOp[0] = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), OpCode[2], not_gate(OpCode[1]),
    not_gate(OpCode[0])) || and_gate6(not_gate(OpCode[5]),
      not_gate(OpCode[4]), OpCode[3], not_gate(OpCode[2]),
      not_gate(OpCode[1]), not_gate(OpCode[0]));
  *Jump = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], not_gate(OpCode[0])) ||
    and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), not_gate(OpCode[3]),
      not_gate(OpCode[2]), OpCode[1], OpCode[0]);
  *JumpReg = and_gate12(not_gate(funct[0]), not_gate(funct[1]), not_gate(funct[2]), funct[3],
    not_gate(funct[4]), not_gate(funct[5]), not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), not_gate(OpCode[1]),
    not_gate(OpCode[0]));
}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  /*
  This function retrieves the data from the two 5-bit
  inputs passed and stores that data in ReadData1 and
  ReadData2, respectively.
  */
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  BIT* O1 = malloc(32);
  BIT* O2 = malloc(32);
  // Similar use of decoder as seen in instruction memory
  decoder5(ReadRegister1, O1);
  decoder5(ReadRegister2, O2);
  for (int i = 0;i < 32;i++) {
    // Take i^th bit of value stored at the register denoted by ReadRegister1
    ReadData1[i] = or_gate32(and_gate(O1[0], MEM_Register[0][i]), and_gate(O1[1], MEM_Register[1][i]),
      and_gate(O1[2], MEM_Register[2][i]), and_gate(O1[3], MEM_Register[3][i]),
      and_gate(O1[4], MEM_Register[4][i]), and_gate(O1[5], MEM_Register[5][i]),
      and_gate(O1[6], MEM_Register[6][i]), and_gate(O1[7], MEM_Register[7][i]),
      and_gate(O1[8], MEM_Register[8][i]), and_gate(O1[9], MEM_Register[9][i]),
      and_gate(O1[10], MEM_Register[10][i]), and_gate(O1[11], MEM_Register[11][i]),
      and_gate(O1[12], MEM_Register[12][i]), and_gate(O1[13], MEM_Register[13][i]),
      and_gate(O1[14], MEM_Register[14][i]), and_gate(O1[15], MEM_Register[15][i]),
      and_gate(O1[16], MEM_Register[16][i]), and_gate(O1[17], MEM_Register[17][i]),
      and_gate(O1[18], MEM_Register[18][i]), and_gate(O1[19], MEM_Register[19][i]),
      and_gate(O1[20], MEM_Register[20][i]), and_gate(O1[21], MEM_Register[21][i]),
      and_gate(O1[22], MEM_Register[22][i]), and_gate(O1[23], MEM_Register[23][i]),
      and_gate(O1[24], MEM_Register[24][i]), and_gate(O1[25], MEM_Register[25][i]),
      and_gate(O1[26], MEM_Register[26][i]), and_gate(O1[27], MEM_Register[27][i]),
      and_gate(O1[28], MEM_Register[28][i]), and_gate(O1[29], MEM_Register[29][i]),
      and_gate(O1[30], MEM_Register[30][i]), and_gate(O1[31], MEM_Register[31][i]));
  };
  for (int i = 0;i < 32;i++) {
    // Take i^th bit of value stored at the register denoted by ReadRegister2
    ReadData2[i] = or_gate32(and_gate(O2[0], MEM_Register[0][i]), and_gate(O2[1], MEM_Register[1][i]),
      and_gate(O2[2], MEM_Register[2][i]), and_gate(O2[3], MEM_Register[3][i]),
      and_gate(O2[4], MEM_Register[4][i]), and_gate(O2[5], MEM_Register[5][i]),
      and_gate(O2[6], MEM_Register[6][i]), and_gate(O2[7], MEM_Register[7][i]),
      and_gate(O2[8], MEM_Register[8][i]), and_gate(O2[9], MEM_Register[9][i]),
      and_gate(O2[10], MEM_Register[10][i]), and_gate(O2[11], MEM_Register[11][i]),
      and_gate(O2[12], MEM_Register[12][i]), and_gate(O2[13], MEM_Register[13][i]),
      and_gate(O2[14], MEM_Register[14][i]), and_gate(O2[15], MEM_Register[15][i]),
      and_gate(O2[16], MEM_Register[16][i]), and_gate(O2[17], MEM_Register[17][i]),
      and_gate(O2[18], MEM_Register[18][i]), and_gate(O2[19], MEM_Register[19][i]),
      and_gate(O2[20], MEM_Register[20][i]), and_gate(O2[21], MEM_Register[21][i]),
      and_gate(O2[22], MEM_Register[22][i]), and_gate(O2[23], MEM_Register[23][i]),
      and_gate(O2[24], MEM_Register[24][i]), and_gate(O2[25], MEM_Register[25][i]),
      and_gate(O2[26], MEM_Register[26][i]), and_gate(O2[27], MEM_Register[27][i]),
      and_gate(O2[28], MEM_Register[28][i]), and_gate(O2[29], MEM_Register[29][i]),
      and_gate(O2[30], MEM_Register[30][i]), and_gate(O2[31], MEM_Register[31][i]));
  };
}

void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData)
{
  /*
  This function writes the data passed in WriteData to the register
  denoted by WriteRegister if RegWrite is set to 1.
  */
  // Input: one 5-bit register address, data to write, and control bit
  // Output: None, but will modify register file
  BIT* O = malloc(32);
  // Identify write register with 5-decoder
  decoder5(WriteRegister, O);
  for (int i = 0;i < 32;i++) {
    for (int j = 0;j < 32;j++) {
      /*
       With the multiplexors, we choose j^th bit of WriteData if
       the i^th register corresponds to WriteRegister. We then choose
       between the contents of WriteData and the original contents
       depending on the value of RegWrite.
       */
      MEM_Register[i][j] = multiplexor2(RegWrite, MEM_Register[i][j],
        multiplexor2(O[i], MEM_Register[i][j], WriteData[j]));
    }
  };

}

void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl)
{
  /*
  This function sets the four ALU control bits based on the
  ALU operation and funct bits derived from the current instruction.
  */
  /*
  - Use SOP expression on each bit
  - Input: 2-bit ALUOp from main control circuit, 6-bit funct field from the
     binary instruction
  - Output:4-bit ALUControl for input into the ALU
  - Note that the SOP expressions are derived from Figure 4.13 on
    page 273 of the text.
  */
  ALUControl[2] = and_gate(ALUOp[0], not_gate(ALUOp[1]))
    || and_gate6(not_gate(ALUOp[0]), ALUOp[1], not_gate(funct[0]), funct[1], not_gate(funct[2]), not_gate(funct[3]))
    || and_gate6(not_gate(ALUOp[0]), ALUOp[1], not_gate(funct[0]), funct[1], not_gate(funct[2]), funct[3]);
  ALUControl[1] = and_gate(not_gate(ALUOp[0]), not_gate(ALUOp[1])) || ALUOp[0]
    || and_gate6(not_gate(ALUOp[0]), ALUOp[1], not_gate(funct[0]), not_gate(funct[1]), not_gate(funct[2]), not_gate(funct[3]))
    || and_gate6(not_gate(ALUOp[0]), ALUOp[1], not_gate(funct[0]), funct[1], not_gate(funct[2]), not_gate(funct[3]))
    || and_gate6(not_gate(ALUOp[0]), ALUOp[1], not_gate(funct[0]), funct[1], not_gate(funct[2]), funct[3])
    || and_gate(ALUOp[0], ALUOp[1]);
  ALUControl[0] = and_gate6(not_gate(ALUOp[0]), ALUOp[1], not_gate(funct[0]), funct[1], not_gate(funct[2]), funct[3])
    || and_gate6(not_gate(ALUOp[0]), ALUOp[1], funct[0], not_gate(funct[1]), funct[2], not_gate(funct[3]));
}

void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result)
{
  /* This function implements the 32-bit ALU in the full datapath.*/
  // Input: 4-bit ALUControl, two 32-bit inputs
  // Output: 32-bit result, and zero flag big
  // Note that most of the solution is from lab 6.
  // Initialize LESS, SET, and CarryOut bits
  BIT Less = FALSE;
  BIT Set = FALSE;
  BIT CarryOut = 0;
  BIT Op0 = ALUControl[0];
  BIT Op1 = ALUControl[1];
  // ALUControl[2] = 1 for sub and slt instructions
  // These require CarryIn and Binvert to be set to 1
  BIT CarryIn = ALUControl[2];
  BIT Binvert = ALUControl[2];
  // Note the use of a 32-bit ripple adder using 1-bit ALUs
  ALU1(Input1[0], Input2[0], Binvert, CarryIn, Less,
    Op0, Op1, &Result[0], &CarryOut, &Set);
  *Zero = Result[0];
  for (int i = 1; i < 32; ++i)
  {
    ALU1(Input1[i], Input2[i], Binvert, CarryOut, Less,
      Op0, Op1, &Result[i], &CarryOut, &Set);
    *Zero = or_gate(*Zero, Result[i]);
  }
  *Zero = not_gate(*Zero);
  Less = Set;
  ALU1(Input1[0], Input2[0], Binvert, CarryIn, Less,
    Op0, Op1, &Result[0], &CarryOut, &Set);
}

void Data_Memory(BIT MemWrite, BIT MemRead,
  BIT* Address, BIT* WriteData, BIT* ReadData)
{
  /*
  This function retrieves from and writes to Data Memory based on the
  given control signals MemWrite and MemRead.
  */
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction

  // Note: Read implementation similar to Register_Read
  BIT* O = malloc(32);
  // Identify target address with 5-decoder
  decoder5(Address, O);
  for (int i = 0;i < 32;i++) {
    // Read data from memory if MemRead is set to 1
    ReadData[i] = and_gate(MemRead, or_gate32(and_gate(O[0], MEM_Data[0][i]), and_gate(O[1], MEM_Data[1][i]),
      and_gate(O[2], MEM_Data[2][i]), and_gate(O[3], MEM_Data[3][i]),
      and_gate(O[4], MEM_Data[4][i]), and_gate(O[5], MEM_Data[5][i]),
      and_gate(O[6], MEM_Data[6][i]), and_gate(O[7], MEM_Data[7][i]),
      and_gate(O[8], MEM_Data[8][i]), and_gate(O[9], MEM_Data[9][i]),
      and_gate(O[10], MEM_Data[10][i]), and_gate(O[11], MEM_Data[11][i]),
      and_gate(O[12], MEM_Data[12][i]), and_gate(O[13], MEM_Data[13][i]),
      and_gate(O[14], MEM_Data[14][i]), and_gate(O[15], MEM_Data[15][i]),
      and_gate(O[16], MEM_Data[16][i]), and_gate(O[17], MEM_Data[17][i]),
      and_gate(O[18], MEM_Data[18][i]), and_gate(O[19], MEM_Data[19][i]),
      and_gate(O[20], MEM_Data[20][i]), and_gate(O[21], MEM_Data[21][i]),
      and_gate(O[22], MEM_Data[22][i]), and_gate(O[23], MEM_Data[23][i]),
      and_gate(O[24], MEM_Data[24][i]), and_gate(O[25], MEM_Data[25][i]),
      and_gate(O[26], MEM_Data[26][i]), and_gate(O[27], MEM_Data[27][i]),
      and_gate(O[28], MEM_Data[28][i]), and_gate(O[29], MEM_Data[29][i]),
      and_gate(O[30], MEM_Data[30][i]), and_gate(O[31], MEM_Data[31][i])));
  };
  // Note: Implementation similar to Register_Write
  for (int i = 0;i < 32;i++) {
    for (int j = 0;j < 32;j++) {
      // Write to memory if MemWrite is set to 1
      MEM_Data[i][j] = multiplexor2(MemWrite, MEM_Data[i][j],
        multiplexor2(O[i], MEM_Data[i][j], WriteData[j]));
    }
  };
}

void Extend_Sign16(BIT* Input, BIT* Output)
{
  /*
  This function extends the a 16-bit input to 32-bits.
  -- Fill higher 16 bits with MSB of 16-bit input
  */
  for (int i = 0;i < 16;i++) {
    Output[i] = Input[i];
  }
  for (int i = 16;i < 32;i++) {
    Output[i] = Input[15];
  }
}

void updateState()
{
  /* This function modifies and progresses the state of the datapath,
  moving between instructions and storing data as the given MIPS
  instructions specify.
  */

  // Prematurely get next instruction value for jal instruction
  // Irrelevant Zero flag (thus labeled garbage)
  BIT ZeroGarbage = 0;
  BIT* newPC = malloc(32);
  BIT ALUControlAdd[4] = { 0,1,0,0 };
  ALU(ALUControlAdd, PC, ONE, &ZeroGarbage, newPC);
  // Fetch - load instruction from instruction memory
  BIT* Instruction = malloc(32);
  Instruction_Memory(PC, Instruction);
  // Opcode bits in the same location for all instructions
  BIT OpCode[6] = { Instruction[26], Instruction[27], Instruction[28],
  Instruction[29], Instruction[30], Instruction[31] };

  // R-type & I-type
  // First source register in the same location for I-type and R-type instructions
  BIT rs1[5] = { Instruction[21], Instruction[22], Instruction[23],
  Instruction[24], Instruction[25] };

  //R-type Instruction bits 
  BIT rs2[5] = { Instruction[16], Instruction[17], Instruction[18],
  Instruction[19], Instruction[20] };
  BIT rd[5] = { Instruction[11], Instruction[12], Instruction[13],
  Instruction[14], Instruction[15] };
  //Note that shamt is unused
  BIT funct[6] = { Instruction[0], Instruction[1], Instruction[2],
   Instruction[3], Instruction[4], Instruction[5] };

  //I-type
  BIT Immediate[16] = { Instruction[0], Instruction[1], Instruction[2],
   Instruction[3], Instruction[4],
      Instruction[5], Instruction[6], Instruction[7], Instruction[8],
       Instruction[9], Instruction[10],
      Instruction[11], Instruction[12], Instruction[13], Instruction[14],
       Instruction[15] };

  //J-type
  BIT ImmediateJ[26] = { Instruction[0], Instruction[1], Instruction[2], Instruction[3], Instruction[4],
      Instruction[5], Instruction[6], Instruction[7], Instruction[8], Instruction[9], Instruction[10],
      Instruction[11], Instruction[12], Instruction[13], Instruction[14], Instruction[15], Instruction[16],
      Instruction[17], Instruction[18], Instruction[19], Instruction[20], Instruction[21], Instruction[22],
      Instruction[23], Instruction[24], Instruction[25] };
  // Decode - set control bits and read from the register file

  //Set control bits
  Control(OpCode, funct,
    &RegDst0, &RegDst1, &Jump, &Branch, &MemRead, &MemToReg0,
    &MemToReg1, ALUOp, &MemWrite, &ALUSrc, &RegWrite, &JumpReg);
  //Set ALU control bits
  BIT* ALUControl = malloc(4);
  ALU_Control(ALUOp, funct, ALUControl);
  //Sign extend the I-type immediate value
  BIT* ImmediateExtended = malloc(32);
  Extend_Sign16(Immediate, ImmediateExtended);
  //Read data from registers
  BIT* ReadData1 = malloc(32);
  BIT* ReadData2 = malloc(32);
  Read_Register(rs1, rs2, ReadData1, ReadData2);
  BIT RegRA[5] = { 1,1,1,1,1 };
  //Potentially write to the write register
  BIT* WriteRegister = malloc(32);
  for (int i = 0;i < 5;i++) {
    // For jal, we store the address of the next instruction to register 31
    // Otherwise, we choose between bits 11-15 or bits 16-20
    WriteRegister[i] = multiplexor4(RegDst0, RegDst1, rs2[i], rd[i], RegRA[i], 0);
  }
  // Execute - process ALU
  BIT* src1 = malloc(32);
  copy_bits(ReadData1, src1);
  BIT* src2 = malloc(32);
  // Choose second ALU source (ReadData2 or the immediate value)
  for (int i = 0;i < 32;i++) {
    src2[i] = multiplexor2(ALUSrc, ReadData2[i], ImmediateExtended[i]);
  }
  BIT* Result = malloc(32);
  BIT Zero = 0;
  // Perform ALU operation
  ALU(ALUControl, src1, src2, &Zero, Result);
  // Memory - read/write data memory
  BIT* ReadData = malloc(32);
  // Potentially read & write from data memory
  Data_Memory(MemWrite, MemRead, Result, ReadData2, ReadData);
  // Write Back - write to the register file
  BIT* WriteData = malloc(32);
  // Identify write data
  for (int i = 0;i < 32;i++) {
    // Choose between data read from D-mem and the ALU result
    WriteData[i] = multiplexor4(MemToReg0, MemToReg1, Result[i], ReadData[i], newPC[i], 0);
  }
  // Write back to the write register if RegWrite = 1
  Write_Register(RegWrite, WriteRegister, WriteData);
  // Update PC - determine the final PC value for the next instruction

  // Branching and jumping
  BIT* BranchAddress = malloc(32);
  // Offset next PC value by branch address
  ALU(ALUControlAdd, ImmediateExtended, newPC, &ZeroGarbage, BranchAddress);
  BIT* JumpAddress = malloc(32);
  copy_bits(ImmediateJ, JumpAddress);
  /*
   As the top 4 bits come from PC, where the top
   4 bits from PC are always 0, we just set the upper
   6 bits of the JumpAddress to 0 as we don't shift
   left by 2 either.*/
  for (int i = 26;i < 32;i++) {
    JumpAddress[i] = 0;
  }
  BIT* BaseAddress = malloc(32);
  // Determine if we should branch
  BIT shouldBranch = and_gate(Zero, Branch);
  // If we should branch, set the PC to the branch address
  multiplexor2_32(shouldBranch, newPC, BranchAddress, BaseAddress);
  // If we should jump, set PC to the jump address
  multiplexor2_32(Jump, BaseAddress, JumpAddress, PC);
  // Jump back to address in ReadData1 if JumpReg = 1
  multiplexor2_32(JumpReg, PC, ReadData1, PC);
  //FREES
  free(newPC);
  free(ALUControl);
  free(ImmediateExtended);
  free(ReadData1);
  free(ReadData2);
  free(WriteRegister);
  free(src1);
  free(src2);
  free(Result);
  free(ReadData);
  free(WriteData);

}

/******************************************************************************/
/* Main */
/******************************************************************************/

int main()
{

  setbuf(stdout, NULL);
  // parse instructions into binary format
  int counter = get_instructions(MEM_Instruction);
  // load program and run
  copy_bits(ZERO, PC);
  copy_bits(THIRTY_TWO, MEM_Register[29]);
  while (binary_to_integer(PC) < counter)
  {
    print_instruction();
    updateState();
    print_state();
  }
  return 0;
}
