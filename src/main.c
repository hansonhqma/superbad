/*
Class Project: The logical conclusion (v1.1)
CSCI-2500 Fall 2021
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
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
void convert_to_binary_char(int a, char* A, int length);
int binary_to_integer(BIT* A);

int get_instructions(BIT Instructions[][32]);

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction);
void Control(BIT* OpCode, BIT funct3,
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
BIT or_gate4(BIT A, BIT B, BIT C, BIT D)
{
  return or_gate(or_gate(A, B), or_gate(C, D));
}
BIT or_gate5(BIT A, BIT B, BIT C, BIT D, BIT E)
{
  //IMPLEMENTED BY: TYLER
  return or_gate(or_gate(or_gate(A, B), or_gate(C, D)), E);
}
BIT or_gate6(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F)
{
  //IMPLEMENTED BY: TYLER
  return or_gate(or_gate(or_gate(A, B), or_gate(C, D)), or_gate(E, F));
}
BIT or_gate7(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F, BIT G)
{
  //IMPLEMENTED BY: TYLER
  return or_gate(or_gate(or_gate(or_gate(A, B), or_gate(C, D)), or_gate(E, F)), G);
}
BIT or_gate8(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F, BIT G, BIT H)
{
  return or_gate(or_gate(or_gate(or_gate(A, B), or_gate(C, D)),
    or_gate(E, F)), or_gate(G, H));
}
//IMPLEMENTED BY: TYLER
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
BIT and_gate4(BIT A, BIT B, BIT C, BIT D)
{
  return and_gate(A, and_gate(B, and_gate(C, D)));
}
BIT and_gate5(BIT A, BIT B, BIT C, BIT D, BIT E)
{
  //IMPLEMENTED BY: TYLER
  return and_gate(and_gate(and_gate(A, B), and_gate(C, D)), E);
}
BIT and_gate6(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F)
{
  //IMPLEMENTED BY: TYLER
  return and_gate(and_gate(and_gate(A, B), and_gate(C, D)), and_gate(E, F));
}
BIT and_gate7(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F, BIT G)
{
  //IMPLEMENTED BY: TYLER
  return and_gate(and_gate(and_gate(and_gate(A, B), and_gate(C, D)), and_gate(E, F)), G);
}
BIT and_gate8(BIT A, BIT B, BIT C, BIT D, BIT E, BIT F, BIT G, BIT H)
{
  return and_gate(and_gate(and_gate(and_gate(A, B), and_gate(C, D)),
    and_gate(E, F)), and_gate(G, H));
}
//IMPLEMENTED BY: TYLER
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
  // Note: The input -> output mapping is slightly modified from before
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(not_gate(I1), I0);
  *O2 = and_gate(I1, not_gate(I0));
  *O3 = and_gate(I1, I0);

  return;
}
void decoder3(BIT* I, BIT EN, BIT* O)
{
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
  BIT EN[4] = { FALSE };
  decoder2(I[3], I[4], &EN[0], &EN[1], &EN[2], &EN[3]);
  decoder3(I, EN[3], &O[24]);
  decoder3(I, EN[2], &O[16]);
  decoder3(I, EN[1], &O[8]);
  decoder3(I, EN[0], &O[0]);
}



BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);
}

void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output)
{
  for (int i = 0; i < 32; ++i)
  {
    BIT x0 = and_gate(not_gate(S), I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
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
  //IMPLEMENTED BY: TYLER
  // TODO: implement a 1-bit adder
  // Note: you can probably copy+paste this from your (or my) Lab 5 solution

  BIT x0 = xor_gate(A, B);
  *Sum = xor_gate(CarryIn, x0);

  BIT y0 = and_gate(x0, CarryIn);
  BIT y1 = and_gate(A, B);
  *CarryOut = or_gate(y0, y1);
}

void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less,
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set)
{
  //IMPLEMENTED BY: TYLER
  // TODO: implement a 1-bit ALU
  // Note: this will need modifications from Lab 5 to account for 'slt'
  // See slide "MSB ALU" in csci2500-f21-ch03a-slides.pdf
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
  //IMPLEMENTED BY: TYLER
  memset(A, '\0', length);
  for (int i = 0; i < length; i++)
  {
    A[i] = a & 1;
    a = a >> 1;
  }
}

void convert_to_binary_char(int a, char* A, int length)
{
  //IMPLEMENTED BY: TYLER
  for (int i = 0; i < length; i++)
  {
    A[i] = a & 1;
    a = a >> 1;
  }
}

int binary_to_integer(BIT* A)
{
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
  //IMPLEMENTED BY: TYLER
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
    //R-type instructions and jr
    return 0;
  }
}
int get_register_number(char* reg)
/*Returns the register number associated with the formatted register*/
{
  //IMPLEMENTED BY: TYLER
  if (strcmp(reg, "zero") == 0)
  {
    return 0;
  }
  else if (strcmp(reg, "v0") == 0)
  {
    return 2;
  }
  else if (strcmp(reg, "a0") == 0)
  {
    return 4;
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
  //IMPLEMENTED BY: TYLER
  char line[256] = { 0 };
  int instruction_count = 0;
  while (fgets(line, 256, stdin) != NULL)
  {
    //Initialize string components of instruction
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
    if (strcmp(instruction, "lw") == 0 || strcmp(instruction, "sw") == 0 ||
      strcmp(instruction, "beq") == 0 || strcmp(instruction, "addi") == 0)
    {
      //I-type instructions

      //Immediate
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
      for (int i = 0; i < 16; i++)
      {
        Instructions[instruction_count][i] = immediate_bits[i];
      }
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 16] = reg2_bits[i];
      }
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 21] = reg1_bits[i];
      }
      for (int i = 0; i < 6; i++)
      {
        Instructions[instruction_count][i + 26] = opcode_bits[i];
      }
    }
    else if (strcmp(instruction, "j") == 0 || strcmp(instruction, "jal") == 0)
    {
      //J-type instructions
      //Immediate
      char* END = NULL;
      int immediate = strtol(dest, &END, 16);
      BIT* immediate_bits = malloc(26);
      convert_to_binary(immediate, immediate_bits, 26);
      //Opcode
      int opcode = get_opcode(instruction);
      BIT* opcode_bits = malloc(6);
      convert_to_binary(opcode, opcode_bits, 6);
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i] = immediate_bits[i];
      }
      for (int i = 0; i < 6; i++)
      {
        Instructions[instruction_count][i + 26] = opcode_bits[i];
      }
    }
    else if (strcmp(instruction, "jr") == 0)
    {
      //handle jr instruction separately
      //Reg1 - Source 1
      int reg1 = get_register_number(dest);
      BIT* reg1_bits = malloc(5);
      convert_to_binary(reg1, reg1_bits, 5);
      //Opcode
      int funct = get_funct(instruction);
      BIT* funct_bits = malloc(6);
      convert_to_binary(funct, funct_bits, 6);
      memset(Instructions[instruction_count], '\0', 32);
      for (int i = 0; i < 6; i++)
      {
        Instructions[instruction_count][i] = funct_bits[i];
      }
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
      for (int i = 0; i < 6; i++)
      {
        Instructions[instruction_count][i] = funct_bits[i];
      }
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 6] = shamt_bits[i];
      }
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 11] = reg1_bits[i];
      }
      for (int i = 0; i < 5; i++)
      {
        Instructions[instruction_count][i + 16] = reg3_bits[i];
      }
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
  // TODO: Implement instruction memory
  // Input: 32-bit instruction address
  // Output: 32-bit binary instruction
  // Note: Useful to use a 5-to-32 decoder here

}

void Control(BIT* OpCode, BIT funct3,
  BIT* RegDst0, BIT* RegDst1, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg0,
  BIT* MemToReg1, BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite, BIT* JumpReg)
{
  /* IMPLEMENTED BY: Tyler
    -- SOP to determine control bits
    -- Introduced four new bits:
      -- JumpReg for jr
      -- RegDst[1] for jal
      -- Jump for j
      -- MemToReg[1] for jal as well
  */
  *RegDst1 = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]);
  *RegDst0 = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), not_gate(OpCode[1]),
    not_gate(OpCode[0]));
  *ALUSrc = and_gate6(OpCode[5], not_gate(OpCode[4]), not_gate(OpCode[3]),
    not_gate(OpCode[2]), not_gate(OpCode[1]), OpCode[0]) ||
    and_gate6(OpCode[5], not_gate(OpCode[4]), OpCode[3],
      not_gate(OpCode[2]), OpCode[1], OpCode[0]) ||
    and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), OpCode[3],
      not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0]));
  *MemToReg1 = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]);
  *MemToReg0 = and_gate6(OpCode[5], not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), not_gate(OpCode[1]), OpCode[0]);
  *RegWrite = and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), not_gate(OpCode[1]),
    not_gate(OpCode[0])) ||
    and_gate6(OpCode[5], not_gate(OpCode[4]), not_gate(OpCode[3]),
      not_gate(OpCode[2]), not_gate(OpCode[1]), OpCode[0]) ||
    and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]),
      not_gate(OpCode[3]), not_gate(OpCode[2]), OpCode[1], OpCode[0]) ||
    and_gate6(not_gate(OpCode[5]), not_gate(OpCode[4]), OpCode[3],
      not_gate(OpCode[2]), not_gate(OpCode[1]), not_gate(OpCode[0]));
  *MemRead = and_gate6(OpCode[5], not_gate(OpCode[4]), not_gate(OpCode[3]),
    not_gate(OpCode[2]), not_gate(OpCode[1]), OpCode[0]);
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
  *JumpReg = and_gate7(funct3, not_gate(OpCode[5]), not_gate(OpCode[4]),
    not_gate(OpCode[3]), not_gate(OpCode[2]), not_gate(OpCode[1]),
    not_gate(OpCode[0]));
}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  //IMPLEMENTED BY: VISHAL & TYLER
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  BIT* O1 = malloc(32);
  BIT* O2 = malloc(32);
  decoder5(ReadRegister1, O1);
  decoder5(ReadRegister2, O2);
  for (int i = 0;i < 32;i++) {
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
  // IMPLEMENTED BY: TYLER
  // Input: one 5-bit register address, data to write, and control bit
  // Output: None, but will modify register file
  BIT* O = malloc(32);
  decoder5(WriteRegister, O);
  for (int i = 0;i < 32;i++) {
    for (int j = 0;j < 32;j++) {
      MEM_Register[i][j] = multiplexor2(RegWrite, MEM_Register[i][j],
        multiplexor2(O[i], MEM_Register[i][j], WriteData[j]));
    }
  };

}

void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl)
{
  /*
  - IMPLEMENTED BY: TYLER
  - Use SOP on each bit
  - Input: 2-bit ALUOp from main control circuit, 6-bit funct field from the
     binary instruction
  - Output:4-bit ALUControl for input into the ALU
  - Note: SOP used to set ALUControl bits
  -- TODO: Account for addi instruction here (which has ALUOp = 11 = 3)
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
  //IMPLEMENTED BY: TYLER
  BIT Less = FALSE;
  BIT Set = FALSE;
  BIT CarryOut = 0;
  BIT Op0 = ALUControl[0];
  BIT Op1 = ALUControl[1];
  /*ALUControl[2] = 1 for sub and slt instructions;
    therefore serves as CarryIn and Binvert bit*/
  BIT CarryIn = ALUControl[2];
  BIT Binvert = ALUControl[2];
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
  // TODO: Implement 32-bit ALU
  // Input: 4-bit ALUControl, two 32-bit inputs
  // Output: 32-bit result, and zero flag big
  // Note: Can re-use prior implementations (but need new circuitry for zero)
}

void Data_Memory(BIT MemWrite, BIT MemRead,
  BIT* Address, BIT* WriteData, BIT* ReadData)
{
  // Implimented by: Rachel Lacobelle

  // TODO: Implement data memory
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction
  // Note: Implementation similar as above

  // use Address and WriteData to find Read Data

  BIT lw_instr = or_gate(MemWrite,MemRead); // will be 1 if it is lw instruction

  // should find the two 5 bit sections to use
  
  
  int count = 0;
  for (int i = 25; i < 21; i--) {
    ReadRegister1[count] = Address[i];
    count += 1;
  }

  count = 0;
  for (int i = 20; i < 16; i--) {
    ReadRegister2[count] = Address[i];
    count += 1;
  }

  Read_Register(ReadRegister1, ReadRegister2, ReadData1, ReadData2);

  for (int i = 0; i < 32; i++) {
    ReadData[i] = WriteData[i];
  }
  

  // Read_Register(BIT* ReadRegister1, BIT* ReadRegister2, BIT* ReadData1, BIT* ReadData2


  // data should only be changed from all 0 if lw instruction (sw can change it as well)
  // output is ReadData
}

void Extend_Sign16(BIT* Input, BIT* Output)
{
  /*
  IMPLEMENTED BY: TYLER
  -- Fill higher 16 bits with MSB of input
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
  unsigned pc = binary_to_integer(PC);
  // TODO: Implement the full datapath here
  // Essentially, you'll be figuring out the order in which to process each of
  // the sub-circuits comprising the entire processor circuit. It makes it
  // easier to consider the pipelined version of the process, and handle things
  // in order of the pipeline. The stages and operations are:
  // Fetch - load instruction from instruction memory
  // Decode - set control bits and read from the register file
  // Execute - process ALU
  // Memory - read/write data memory
  // Write Back - write to the register file
  // Update PC - determine the final PC value for the next instruction
  BIT* newPC = malloc(32);
  BIT ALUControl[4] = { 0,1,0,0 };
  BIT* one = malloc(32);
  convert_to_binary(1, one, 32);
  ALU(ALUControl, PC, one, &Zero, newPC);
  copy_bits(newPC, PC);
  free(newPC);

}

/******************************************************************************/
/* Main */
/******************************************************************************/

int main()
{
  setbuf(stdout, NULL);
  //Read register test
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
