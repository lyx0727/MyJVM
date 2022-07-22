#include "math.h"
#include "stack.h"
#include "control.h"
#include "constants.h"
#include "loads.h"
#include "stores.h"
#include "comparisons.h"
#include "conversions.h"
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
	// { 0x12, new &LDC{} },
	// { 0x13, new &LDC_W{} },
	// { 0x14, new &LDC2_W{} },
	{ 0x15, new ILOAD }, { 0x16, new LLOAD }, { 0x17, new FLOAD }, { 0x18, new DLOAD }, { 0x19, new ALOAD },
	{ 0x1a, new ILOAD_0 }, { 0x1b, new ILOAD_1 }, { 0x1c, new ILOAD_2 }, { 0x1d, new ILOAD_3 },
	{ 0x1e, new LLOAD_0 }, { 0x1f, new LLOAD_1 }, { 0x20, new LLOAD_2 }, { 0x21, new LLOAD_3 },
	{ 0x22, new FLOAD_0 }, { 0x23, new FLOAD_1 }, { 0x24, new FLOAD_2 }, { 0x25, new FLOAD_3 },
	{ 0x26, new DLOAD_0 }, { 0x27, new DLOAD_1 }, { 0x28, new DLOAD_2 }, { 0x29, new DLOAD_3 },
	{ 0x2a, new ALOAD_0 }, { 0x2b, new ALOAD_1 }, { 0x2c, new ALOAD_2 }, { 0x2d, new ALOAD_3 },
	// { 0x2e, new IALOAD },
	// { 0x2f, new LALOAD },
	// { 0x30, new FALOAD },
	// { 0x31, new DALOAD },
	// { 0x32, new AALOAD },
	// { 0x33, new BALOAD },
	// { 0x34, new CALOAD },
	// { 0x35, new SALOAD },
	// stores
	{ 0x36, new ISTORE }, { 0x37, new LSTORE }, { 0x38, new FSTORE }, { 0x39, new DSTORE }, { 0x3a, new ASTORE },
	{ 0x3b, new ISTORE_0 }, { 0x3c, new ISTORE_1 }, { 0x3d, new ISTORE_2 }, { 0x3e, new ISTORE_3 },
	{ 0x3f, new LSTORE_0 }, { 0x40, new LSTORE_1 }, { 0x41, new LSTORE_2 }, { 0x42, new LSTORE_3 },
	{ 0x43, new FSTORE_0 }, { 0x44, new FSTORE_1 }, { 0x45, new FSTORE_2 }, { 0x46, new FSTORE_3 },
	{ 0x47, new DSTORE_0 }, { 0x48, new DSTORE_1 }, { 0x49, new DSTORE_2 }, { 0x4a, new DSTORE_3 },
	{ 0x4b, new ASTORE_0 }, { 0x4c, new ASTORE_1 }, { 0x4d, new ASTORE_2 }, { 0x4e, new ASTORE_3 },
	// { 0x4f, new IASTORE },
	// { 0x50, new LASTORE },
	// { 0x51, new FASTORE },
	// { 0x52, new DASTORE },
	// { 0x53, new AASTORE },
	// { 0x54, new BASTORE },
	// { 0x55, new CASTORE },
	// { 0x56, new SASTORE },
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
	// { 0xac, new ireturn },
	// { 0xad, new lreturn },
	// { 0xae, new freturn },
	// { 0xaf, new dreturn },
	// { 0xb0, new areturn },
	// { 0xb1, new _return },
	// { 0xb2, new &GetStatic{} },
	// { 0xb3, new &PupStatic{} },
	// { 0xb4, new &GetField{} },
	// { 0xb5, new &PutField{} },
	// { 0xb6, new &InvokeVirtual{} },
	// { 0xb7, new &InvokeSpecial{} },
	// { 0xb8, new &InvokeStatic{} },
	// { 0xb9, new &InvokeInterface{} },
	// { 0xba, new &InvokeDynamic{} },
	// { 0xbb, new &New{} },
	// { 0xbc, new &NewArray{} },
	// { 0xbd, new &ANewArray{} },
	// { 0xbe, new arraylength },
	// { 0xbf, new athrow },
	// { 0xc0, new &CheckCast{} },
	// { 0xc1, new &InstanceOf{} },
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