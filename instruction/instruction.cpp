#include "math.h"
#include "stack.h"
#include "control.h"
#include "constants.h"
#include "loads.h"
#include "stores.h"
#include "comparisons.h"
#include "conversions.h"
#include "references.h"
using namespace std;

// TODO
const unordered_map<uint8_t, Instruction*> Instructions = {
	// constants
	{0x00, new NOP}, {0x01, new ACONST_NULL},
	{ 0x02, new ICONST_M1 }, { 0x03, new ICONST_0 }, { 0x04, new ICONST_1 }, { 0x05, new ICONST_2 }, { 0x06, new ICONST_3 }, { 0x07, new ICONST_4 }, { 0x08, new ICONST_5 },
	{ 0x09, new LCONST_0 }, { 0x0a, new LCONST_1 },
	{ 0x0b, new FCONST_0 }, { 0x0c, new FCONST_1 }, { 0x0d, new FCONST_2 },
	{ 0x0e, new DCONST_0 }, { 0x0f, new DCONST_1 },
	{ 0x10, new BIPUSH }, { 0x11, new SIPUSH },
	// load s
	{ 0x12, new LDC },
	{ 0x13, new LDC_W },
	{ 0x14, new LDC2_W },
	{ 0x15, new ILOAD }, { 0x16, new LLOAD }, { 0x17, new FLOAD }, { 0x18, new DLOAD }, { 0x19, new ALOAD },
	{ 0x1a, new ILOAD_0 }, { 0x1b, new ILOAD_1 }, { 0x1c, new ILOAD_2 }, { 0x1d, new ILOAD_3 },
	{ 0x1e, new LLOAD_0 }, { 0x1f, new LLOAD_1 }, { 0x20, new LLOAD_2 }, { 0x21, new LLOAD_3 },
	{ 0x22, new FLOAD_0 }, { 0x23, new FLOAD_1 }, { 0x24, new FLOAD_2 }, { 0x25, new FLOAD_3 },
	{ 0x26, new DLOAD_0 }, { 0x27, new DLOAD_1 }, { 0x28, new DLOAD_2 }, { 0x29, new DLOAD_3 },
	{ 0x2a, new ALOAD_0 }, { 0x2b, new ALOAD_1 }, { 0x2c, new ALOAD_2 }, { 0x2d, new ALOAD_3 },
	// load array
	{ 0x2e, new IALOAD }, { 0x2f, new LALOAD }, { 0x30, new FALOAD }, { 0x31, new DALOAD }, { 0x32, new AALOAD }, { 0x33, new BALOAD }, { 0x34, new CALOAD }, { 0x35, new SALOAD },
	// stores
	{ 0x36, new ISTORE }, { 0x37, new LSTORE }, { 0x38, new FSTORE }, { 0x39, new DSTORE }, { 0x3a, new ASTORE },
	{ 0x3b, new ISTORE_0 }, { 0x3c, new ISTORE_1 }, { 0x3d, new ISTORE_2 }, { 0x3e, new ISTORE_3 },
	{ 0x3f, new LSTORE_0 }, { 0x40, new LSTORE_1 }, { 0x41, new LSTORE_2 }, { 0x42, new LSTORE_3 },
	{ 0x43, new FSTORE_0 }, { 0x44, new FSTORE_1 }, { 0x45, new FSTORE_2 }, { 0x46, new FSTORE_3 },
	{ 0x47, new DSTORE_0 }, { 0x48, new DSTORE_1 }, { 0x49, new DSTORE_2 }, { 0x4a, new DSTORE_3 },
	{ 0x4b, new ASTORE_0 }, { 0x4c, new ASTORE_1 }, { 0x4d, new ASTORE_2 }, { 0x4e, new ASTORE_3 },
	// store array
	{ 0x4f, new IASTORE }, { 0x50, new LASTORE }, { 0x51, new FASTORE }, { 0x52, new DASTORE }, { 0x53, new AASTORE }, { 0x54, new BASTORE }, { 0x55, new CASTORE }, { 0x56, new SASTORE },
	// stack
	{ 0x57, new POP }, { 0x58, new POP2 },
	{ 0x59, new DUP },
	// { 0x5a, new DUP_X1 },
	// { 0x5b, new DUP_X2 },
	{ 0x5c, new DUP2 },
	// { 0x5d, new DUP2_X1 },
	// { 0x5e, new DUP2_X2 },
	{ 0x5f, new SWAP },
	// math
	{ 0x60, new IADD }, { 0x61, new LADD }, { 0x62, new FADD }, { 0x63, new DADD },
	{ 0x64, new ISUB }, { 0x65, new LSUB }, { 0x66, new FSUB }, { 0x67, new DSUB },
	{ 0x68, new IMUL }, { 0x69, new LMUL }, { 0x6a, new FMUL }, { 0x6b, new DMUL },
	{ 0x6c, new IDIV }, { 0x6d, new LDIV }, { 0x6e, new FDIV }, { 0x6f, new DDIV },
	{ 0x70, new IREM }, { 0x71, new LREM }, { 0x72, new FREM }, { 0x73, new DREM },
	{ 0x74, new INEG }, { 0x75, new LNEG }, { 0x76, new FNEG }, { 0x77, new DNEG },
	{ 0x78, new ISHL }, { 0x79, new LSHL },
	{ 0x7a, new ISHR }, { 0x7b, new LSHR },
	{ 0x7c, new IUSHR }, { 0x7d, new LUSHR },
	{ 0x7e, new IAND }, { 0x7f, new LAND },
	{ 0x80, new IOR }, { 0x81, new LOR },
	{ 0x82, new IXOR }, { 0x83, new LXOR },
	{ 0x84, new IINC },
	// conversions
	{ 0x85, new I2L }, { 0x86, new I2F }, { 0x87, new I2D },
	{ 0x88, new L2I }, { 0x89, new L2F }, { 0x8a, new L2D },
	{ 0x8b, new F2I }, { 0x8c, new F2L }, { 0x8d, new F2D },
	{ 0x8e, new D2I }, { 0x8f, new D2L }, { 0x90, new D2F },
	// { 0x91, new I2B },
	// { 0x92, new I2C },
	// { 0x93, new I2S },
	// comparisons
	{ 0x94, new LCMP },
	{ 0x95, new FCMPL }, { 0x96, new FCMPG },
	{ 0x97, new DCMPL }, { 0x98, new DCMPG },
	{ 0x99, new IFEQ }, { 0x9a, new IFNE }, { 0x9b, new IFLT }, { 0x9c, new IFGE }, { 0x9d, new IFGT }, { 0x9e, new IFLE },
	{ 0x9f, new IF_ICMPEQ }, { 0xa0, new IF_ICMPNE }, { 0xa1, new IF_ICMPLT }, { 0xa2, new IF_ICMPGE }, { 0xa3, new IF_ICMPGT }, { 0xa4, new IF_ICMPLE },
	{ 0xa5, new IF_ACMPEQ }, { 0xa6, new IF_ACMPNE },
	// control
	{ 0xa7, new GOTO },
	// { 0xa8, new &JSR{} },
	// { 0xa9, new &RET{} },
	{ 0xaa, new TABLE_SWITCH }, { 0xab, new LOOKUP_SWITCH },
	{ 0xac, new IRETURN }, { 0xad, new LRETURN }, { 0xae, new FRETURN }, { 0xaf, new DRETURN }, { 0xb0, new ARETURN }, { 0xb1, new RETURN },
	{ 0xb2, new GET_STATIC },
	{ 0xb3, new PUT_STATIC },
	{ 0xb4, new GET_FIELD },
	{ 0xb5, new PUT_FIELD },
	{ 0xb6, new INVOKE_VIRTUAL},
	{ 0xb7, new INVOKE_SPECIAL },
	{ 0xb8, new INVOKE_STATIC },
	{ 0xb9, new INVOKE_INTERFACE },
	// { 0xba, new &InvokeDynamic{} },
	// references
	{ 0xbb, new NEW },
	// array
	{ 0xbc, new NEW_ARRAY }, { 0xbd, new ANEW_ARRAY }, { 0xbe, new ARRAY_LENGTH },
	// { 0xbf, new athrow },
	{ 0xc0, new CHECK_CAST }, { 0xc1, new INSTANCE_OF },
	// { 0xc2, new monitorenter },
	// { 0xc3, new monitorexit },
	// { 0xc4, new &Wide{} },
	// { 0xc5, new &MultiANewArray{} },
	// { 0xc6, new NewIfNull() },
	// { 0xc7, new NewIfNonNull() },
	// { 0xc8, new &GotoW{} },
	// { 0xc9, new &JSR_W{} },
	// // case 0xca: todo breakpoint
	// { 0xfe, new invoke_native },	// impdep1
	// { 0xff, new &Bootstrap{} }, 	// impdep2
};

const unordered_map<uint8_t, string> InstructionNames = {
	// constants
	{0x00, "NOP"}, {0x01, "ACONST_NULL"},
	{ 0x02, "ICONST_M1 "}, { 0x03, "ICONST_0 "}, { 0x04, "ICONST_1 "}, { 0x05, "ICONST_2 "}, { 0x06, "ICONST_3 "}, { 0x07, "ICONST_4 "}, { 0x08, "ICONST_5 "},
	{ 0x09, "LCONST_0 "}, { 0x0a, "LCONST_1 "},
	{ 0x0b, "FCONST_0 "}, { 0x0c, "FCONST_1 "}, { 0x0d, "FCONST_2 "},
	{ 0x0e, "DCONST_0 "}, { 0x0f, "DCONST_1 "},
	{ 0x10, "BIPUSH "}, { 0x11, "SIPUSH "},
	// load s
	{ 0x12, "LDC "},
	{ 0x13, "LDC_W "},
	{ 0x14, "LDC2_W "},
	{ 0x15, "ILOAD "}, { 0x16, "LLOAD "}, { 0x17, "FLOAD "}, { 0x18, "DLOAD "}, { 0x19, "ALOAD "},
	{ 0x1a, "ILOAD_0 "}, { 0x1b, "ILOAD_1 "}, { 0x1c, "ILOAD_2 "}, { 0x1d, "ILOAD_3 "},
	{ 0x1e, "LLOAD_0 "}, { 0x1f, "LLOAD_1 "}, { 0x20, "LLOAD_2 "}, { 0x21, "LLOAD_3 "},
	{ 0x22, "FLOAD_0 "}, { 0x23, "FLOAD_1 "}, { 0x24, "FLOAD_2 "}, { 0x25, "FLOAD_3 "},
	{ 0x26, "DLOAD_0 "}, { 0x27, "DLOAD_1 "}, { 0x28, "DLOAD_2 "}, { 0x29, "DLOAD_3 "},
	{ 0x2a, "ALOAD_0 "}, { 0x2b, "ALOAD_1 "}, { 0x2c, "ALOAD_2 "}, { 0x2d, "ALOAD_3 "},
	{ 0x2e, "IALOAD "}, { 0x2f, "LALOAD "}, { 0x30, "FALOAD "}, { 0x31, "DALOAD "}, { 0x32, "AALOAD "}, { 0x33, "BALOAD "}, { 0x34, "CALOAD "}, { 0x35, "SALOAD "},
	// stores
	{ 0x36, "ISTORE "}, { 0x37, "LSTORE "}, { 0x38, "FSTORE "}, { 0x39, "DSTORE "}, { 0x3a, "ASTORE "},
	{ 0x3b, "ISTORE_0 "}, { 0x3c, "ISTORE_1 "}, { 0x3d, "ISTORE_2 "}, { 0x3e, "ISTORE_3 "},
	{ 0x3f, "LSTORE_0 "}, { 0x40, "LSTORE_1 "}, { 0x41, "LSTORE_2 "}, { 0x42, "LSTORE_3 "},
	{ 0x43, "FSTORE_0 "}, { 0x44, "FSTORE_1 "}, { 0x45, "FSTORE_2 "}, { 0x46, "FSTORE_3 "},
	{ 0x47, "DSTORE_0 "}, { 0x48, "DSTORE_1 "}, { 0x49, "DSTORE_2 "}, { 0x4a, "DSTORE_3 "},
	{ 0x4b, "ASTORE_0 "}, { 0x4c, "ASTORE_1 "}, { 0x4d, "ASTORE_2 "}, { 0x4e, "ASTORE_3 "},
	{ 0x4f, "IASTORE "}, { 0x50, "LASTORE "}, { 0x51, "FASTORE "}, { 0x52, "DASTORE "}, { 0x53, "AASTORE "}, { 0x54, "BASTORE "}, { 0x55, "CASTORE "}, { 0x56, "SASTORE "},
	// stack
	{ 0x57, "POP "}, { 0x58, "POP2 "},
	{ 0x59, "DUP "}, { 0x5a, "DUP_X1 "}, { 0x5b, "DUP_X2 "},
	{ 0x5c, "DUP2 "}, { 0x5d, "DUP2_X1 "}, { 0x5e, "DUP2_X2 "},
	{ 0x5f, "SWAP "},
	// math
	{ 0x60, "IADD "}, { 0x61, "LADD "}, { 0x62, "FADD "}, { 0x63, "DADD "},
	{ 0x64, "ISUB "}, { 0x65, "LSUB "}, { 0x66, "FSUB "}, { 0x67, "DSUB "},
	{ 0x68, "IMUL "}, { 0x69, "LMUL "}, { 0x6a, "FMUL "}, { 0x6b, "DMUL "},
	{ 0x6c, "IDIV "}, { 0x6d, "LDIV "}, { 0x6e, "FDIV "}, { 0x6f, "DDIV "},
	{ 0x70, "IREM "}, { 0x71, "LREM "}, { 0x72, "FREM "}, { 0x73, "DREM "},
	{ 0x74, "INEG "}, { 0x75, "LNEG "}, { 0x76, "FNEG "}, { 0x77, "DNEG "},
	{ 0x78, "ISHL "}, { 0x79, "LSHL "},
	{ 0x7a, "ISHR "}, { 0x7b, "LSHR "},
	{ 0x7c, "IUSHR "}, { 0x7d, "LUSHR "},
	{ 0x7e, "IAND "}, { 0x7f, "LAND "},
	{ 0x80, "IOR "}, { 0x81, "LOR "},
	{ 0x82, "IXOR "}, { 0x83, "LXOR "},
	{ 0x84, "IINC "},
	// conversions
	{ 0x85, "I2L "}, { 0x86, "I2F "}, { 0x87, "I2D "},
	{ 0x88, "L2I "}, { 0x89, "L2F "}, { 0x8a, "L2D "},
	{ 0x8b, "F2I "}, { 0x8c, "F2L "}, { 0x8d, "F2D "},
	{ 0x8e, "D2I "}, { 0x8f, "D2L "}, { 0x90, "D2F "},
	{ 0x91, "I2B "}, { 0x92, "I2C "}, { 0x93, "I2S "},
	// comparisons
	{ 0x94, "LCMP "},
	{ 0x95, "FCMPL "}, { 0x96, "FCMPG "},
	{ 0x97, "DCMPL "}, { 0x98, "DCMPG "},
	{ 0x99, "IFEQ "}, { 0x9a, "IFNE "}, { 0x9b, "IFLT "}, { 0x9c, "IFGE "}, { 0x9d, "IFGT "}, { 0x9e, "IFLE "},
	{ 0x9f, "IF_ICMPEQ "}, { 0xa0, "IF_ICMPNE "}, { 0xa1, "IF_ICMPLT "}, { 0xa2, "IF_ICMPGE "}, { 0xa3, "IF_ICMPGT "}, { 0xa4, "IF_ICMPLE "},
	{ 0xa5, "IF_ACMPEQ "}, { 0xa6, "IF_ACMPNE "},
	// control
	{ 0xa7, "GOTO "},
	{ 0xa8, "JSR"},
	{ 0xa9, "RET"},
	{ 0xaa, "TABLE_SWITCH "}, { 0xab, "LOOKUP_SWITCH "},
	{ 0xac, "IRETURN "}, { 0xad, "LRETURN "}, { 0xae, "FRETURN "}, { 0xaf, "DRETURN "}, { 0xb0, "ARETURN "}, { 0xb1, "RETURN "},
	{ 0xb2, "GET_STATIC "},
	{ 0xb3, "PUT_STATIC "},
	{ 0xb4, "GET_FIELD "},
	{ 0xb5, "PUT_FIELD "},
	{ 0xb6, "INVOKE_VIRTUAL"},
	{ 0xb7, "INVOKE_SPECIAL "},
	{ 0xb8, "INVOKE_STATIC"},
	{ 0xb9, "INVOKE_INTERFACE"},
	{ 0xba, "INVOKE_DYNAMIC"},
	// references
	{ 0xbb, "NEW"},
	{ 0xbc, "NEW_ARRAY"},
	{ 0xbd, "ANEW_ARRAY"},
	{ 0xbe, "ARRAY_LENGTH"},
	{ 0xbf, "ATHROW"},
	{ 0xc0, "CHECK_CAST"},
	{ 0xc1, "INSTANCE_OF"},
	// { 0xc2, "monitorenter "},
	// { 0xc3, "monitorexit "},
	// { 0xc4, "&Wide{"} "},
	// { 0xc5, "&MultiANewArray{"} "},
	// { 0xc6, "NewIfNull() "},
	// { 0xc7, "NewIfNonNull() "},
	// { 0xc8, "&GotoW{"} "},
	// { 0xc9, "&JSR_W{"} "},
	// // case 0xca: todo breakpoint
	// { 0xfe, "invoke_native "},	// impdep1
	// { 0xff, "&Bootstrap{"} "}, 	// impdep2
};

Instruction* getInstrucion(uint8_t opCode){
	if(Instructions.count(opCode)){
		return Instructions.at(opCode);
	}
	else{
		cerr << "invalid opcode: " << (int)opCode << endl;
		// exit(1);
	}
	return nullptr;
}

const string getInstrucionName(uint8_t opCode){
	if(InstructionNames.count(opCode)){
		return InstructionNames.at(opCode);
	}
	else{
		cerr << "invalid opcode: " << (int)opCode << endl;
		// exit(1);
	}
	return "";
}