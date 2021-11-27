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
void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite);
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
    /*DEBUG
    for (int i = 31; i >= 0; i--)
    {
      printf("%d", Instructions[instruction_count][i]);
    }
    printf("\n");
    */
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

BIT RegDst = FALSE;
BIT Jump = FALSE;
BIT Branch = FALSE;
BIT MemRead = FALSE;
BIT MemToReg = FALSE;
BIT ALUOp[2] = { FALSE };
BIT MemWrite = FALSE;
BIT ALUSrc = FALSE;
BIT RegWrite = FALSE;
BIT Zero = FALSE;
BIT ALUControl[4] = { FALSE };

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

void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite)
{
  // TODO: Set control bits for everything
  // Input: opcode field from the instruction
  // OUtput: all control lines get set
  // Note: Can use SOP or similar approaches to determine bits
}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  // TODO: Implement register read functionality
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  // Note: Implementation will be very similar to instruction memory circuit
}

void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData)
{
  // TODO: Implement register write functionality
  // Input: one 5-bit register address, data to write, and control bit
  // Output: None, but will modify register file
  // Note: Implementation will again be similar to those above
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
  */
  ALUControl[2] = ALUOp[0]
    || and_gate5(ALUOp[1], not_gate(funct[0]), funct[1], not_gate(funct[2]), not_gate(funct[3]))
    || and_gate5(ALUOp[1], not_gate(funct[0]), funct[1], not_gate(funct[2]), funct[3]);
  ALUControl[1] = and_gate(not_gate(ALUOp[0]), not_gate(ALUOp[1])) || ALUOp[0]
    || and_gate5(ALUOp[1], not_gate(funct[0]), not_gate(funct[1]), not_gate(funct[2]), not_gate(funct[3]))
    || and_gate5(ALUOp[1], not_gate(funct[0]), funct[1], not_gate(funct[2]), not_gate(funct[3]))
    || and_gate5(ALUOp[1], not_gate(funct[0]), funct[1], not_gate(funct[2]), funct[3]);
  ALUControl[0] = and_gate5(ALUOp[1], not_gate(funct[0]), funct[1], not_gate(funct[2]), funct[3])
    || and_gate5(ALUOp[1], funct[0], not_gate(funct[1]), funct[2], not_gate(funct[3]));
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
  // TODO: Implement data memory
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction
  // Note: Implementation similar as above
}

void Extend_Sign16(BIT* Input, BIT* Output)
{
  // TODO: Implement 16-bit to 32-bit sign extender
  // Copy Input to Output, then extend 16th Input bit to 17-32 bits in Output
}

void updateState()
{
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
  /*
  while (binary_to_integer(PC) < counter)
  {
    print_instruction();
    updateState();
    print_state();
  }*/

  return 0;
}