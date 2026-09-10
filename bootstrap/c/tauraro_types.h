#pragma once
#define TAURARO_STD_LIB
#define TAURARO_RT_NO_STRINGBUILDER
#include "tauraro_rt.h"

typedef struct StringObj StringObj;
typedef struct StringBuilder StringBuilder;
typedef struct Lexer Lexer;
typedef struct AstType AstType;
typedef struct GenericConstraint GenericConstraint;
typedef struct Decorator Decorator;
typedef struct Comprehension Comprehension;
typedef struct CatchClause CatchClause;
typedef struct MatchArm MatchArm;
typedef struct FStringPart FStringPart;
typedef struct ChanSelectArm ChanSelectArm;
typedef struct Block Block;
typedef struct ElifClause ElifClause;
typedef struct Param Param;
typedef struct FunctionDef FunctionDef;
typedef struct FieldDef FieldDef;
typedef struct ClassDef ClassDef;
typedef struct VariantDef VariantDef;
typedef struct EnumDef EnumDef;
typedef struct InterfaceDef InterfaceDef;
typedef struct ImportItem ImportItem;
typedef struct Program Program;
typedef struct Parser Parser;
typedef struct ModuleResolver ModuleResolver;
typedef struct HirComprehension HirComprehension;
typedef struct HirCatchClause HirCatchClause;
typedef struct HirFStringPart HirFStringPart;
typedef struct HirMatchArm HirMatchArm;
typedef struct HirChanSelectArm HirChanSelectArm;
typedef struct HirBlock HirBlock;
typedef struct HirParam HirParam;
typedef struct HirFunction HirFunction;
typedef struct HirField HirField;
typedef struct HirClass HirClass;
typedef struct HirVariant HirVariant;
typedef struct HirEnum HirEnum;
typedef struct HirInterface HirInterface;
typedef struct HirProgram HirProgram;
typedef struct MirBlock MirBlock;
typedef struct DropSite DropSite;
typedef struct BorrowEdge BorrowEdge;
typedef struct MirFunction MirFunction;
typedef struct MirProgram MirProgram;
typedef struct MirBuilder MirBuilder;
typedef struct LiveSet LiveSet;
typedef struct Symbol Symbol;
typedef struct Scope Scope;
typedef struct Sema Sema;
typedef struct Formatter Formatter;
typedef struct CTok CTok;
typedef struct Bindgen Bindgen;
typedef struct CppType CppType;
typedef struct CGenerator CGenerator;
typedef struct CppExporter CppExporter;
typedef struct LBlock LBlock;
typedef struct LFunc LFunc;
typedef struct ClassLayout ClassLayout;
typedef struct VariantLayout VariantLayout;
typedef struct EnumLayout EnumLayout;
typedef struct LModule LModule;
typedef struct LlvmEmitter LlvmEmitter;
typedef struct LlvmGenerator LlvmGenerator;
typedef struct ByteBuf ByteBuf;
typedef struct Reloc Reloc;
typedef struct Jump Jump;
typedef struct EncodedFunc EncodedFunc;
typedef struct TextReloc TextReloc;
typedef struct NativeGenerator NativeGenerator;
typedef struct GVal GVal;
typedef struct GpuEmitter GpuEmitter;
typedef struct GpuGenerator GpuGenerator;
typedef struct MacroCtx MacroCtx;
typedef struct FnMacroExpander FnMacroExpander;
typedef struct Token Token;
typedef struct Pattern Pattern;
typedef struct Ownership Ownership;
typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct Decl Decl;
typedef struct HirExpr HirExpr;
typedef struct HirStmt HirStmt;
typedef struct MirStmt MirStmt;
typedef struct MirTerm MirTerm;
typedef struct SymbolKind SymbolKind;
typedef struct LType LType;
typedef struct LInst LInst;
typedef struct LTerm LTerm;
typedef struct MacroVal MacroVal;
typedef struct List_Token List_Token;
typedef struct List_Pattern List_Pattern;


static void _trdrop_StringObj(void* vp);
static void _trdrop_Lexer(void* vp);
static void _trdrop_AstType(void* vp);
static void _trdrop_GenericConstraint(void* vp);
static void _trdrop_Decorator(void* vp);
static void _trdrop_Comprehension(void* vp);
static void _trdrop_CatchClause(void* vp);
static void _trdrop_MatchArm(void* vp);
static void _trdrop_FStringPart(void* vp);
static void _trdrop_ChanSelectArm(void* vp);
static void _trdrop_Block(void* vp);
static void _trdrop_ElifClause(void* vp);
static void _trdrop_Param(void* vp);
static void _trdrop_FunctionDef(void* vp);
static void _trdrop_FieldDef(void* vp);
static void _trdrop_ClassDef(void* vp);
static void _trdrop_VariantDef(void* vp);
static void _trdrop_EnumDef(void* vp);
static void _trdrop_InterfaceDef(void* vp);
static void _trdrop_ImportItem(void* vp);
static void _trdrop_Program(void* vp);
static void _trdrop_Parser(void* vp);
static void _trdrop_ModuleResolver(void* vp);
static void _trdrop_HirComprehension(void* vp);
static void _trdrop_HirCatchClause(void* vp);
static void _trdrop_HirFStringPart(void* vp);
static void _trdrop_HirMatchArm(void* vp);
static void _trdrop_HirChanSelectArm(void* vp);
static void _trdrop_HirBlock(void* vp);
static void _trdrop_HirParam(void* vp);
static void _trdrop_HirFunction(void* vp);
static void _trdrop_HirField(void* vp);
static void _trdrop_HirClass(void* vp);
static void _trdrop_HirVariant(void* vp);
static void _trdrop_HirEnum(void* vp);
static void _trdrop_HirInterface(void* vp);
static void _trdrop_HirProgram(void* vp);
static void _trdrop_MirBlock(void* vp);
static void _trdrop_DropSite(void* vp);
static void _trdrop_BorrowEdge(void* vp);
static void _trdrop_MirFunction(void* vp);
static void _trdrop_MirProgram(void* vp);
static void _trdrop_MirBuilder(void* vp);
static void _trdrop_LiveSet(void* vp);
static void _trdrop_Symbol(void* vp);
static void _trdrop_Scope(void* vp);
static void _trdrop_Sema(void* vp);
static void _trdrop_Formatter(void* vp);
static void _trdrop_CTok(void* vp);
static void _trdrop_Bindgen(void* vp);
static void _trdrop_CppType(void* vp);
static void _trdrop_CGenerator(void* vp);
static void _trdrop_CppExporter(void* vp);
static void _trdrop_LBlock(void* vp);
static void _trdrop_LFunc(void* vp);
static void _trdrop_ClassLayout(void* vp);
static void _trdrop_VariantLayout(void* vp);
static void _trdrop_EnumLayout(void* vp);
static void _trdrop_LModule(void* vp);
static void _trdrop_LlvmEmitter(void* vp);
static void _trdrop_LlvmGenerator(void* vp);
static void _trdrop_ByteBuf(void* vp);
static void _trdrop_Reloc(void* vp);
static void _trdrop_Jump(void* vp);
static void _trdrop_EncodedFunc(void* vp);
static void _trdrop_TextReloc(void* vp);
static void _trdrop_NativeGenerator(void* vp);
static void _trdrop_GVal(void* vp);
static void _trdrop_GpuEmitter(void* vp);
static void _trdrop_GpuGenerator(void* vp);
static void _trdrop_MacroCtx(void* vp);
static void _trdrop_FnMacroExpander(void* vp);

typedef enum {
    Token_IntLit,
    Token_FloatLit,
    Token_StrLit,
    Token_TripleStrLit,
    Token_ByteStrLit,
    Token_RawStrLit,
    Token_CharLit,
    Token_FStrLit,
    Token_BoolLit,
    Token_Ident,
    Token_KwDef,
    Token_KwClass,
    Token_KwEnum,
    Token_KwInterface,
    Token_KwExtend,
    Token_KwStruct,
    Token_KwIf,
    Token_KwElif,
    Token_KwElse,
    Token_KwFor,
    Token_KwWhile,
    Token_KwLoop,
    Token_KwReturn,
    Token_KwBreak,
    Token_KwContinue,
    Token_KwMatch,
    Token_KwCase,
    Token_KwTry,
    Token_KwAssert,
    Token_KwWith,
    Token_KwDefer,
    Token_KwAsm,
    Token_KwTaskGroup,
    Token_KwGpu,
    Token_KwSizeOf,
    Token_KwExcept,
    Token_KwFinally,
    Token_KwRaise,
    Token_KwPass,
    Token_KwImplements,
    Token_KwImport,
    Token_KwFrom,
    Token_KwAs,
    Token_KwIn,
    Token_KwMut,
    Token_KwShared,
    Token_KwExtern,
    Token_KwUnsafe,
    Token_KwSpawn,
    Token_KwAsync,
    Token_KwThrows,
    Token_KwExtends,
    Token_KwAwait,
    Token_KwYield,
    Token_KwPub,
    Token_KwWhere,
    Token_KwStatic,
    Token_KwStack,
    Token_KwOwn,
    Token_KwBorrow,
    Token_KwMove,
    Token_KwConst,
    Token_KwActor,
    Token_KwSuper,
    Token_KwExport,
    Token_KwLambda,
    Token_KwDecorator,
    Token_KwMacro,
    Token_KwDo,
    Token_KwTrue,
    Token_KwFalse,
    Token_KwNone,
    Token_KwAnd,
    Token_KwOr,
    Token_KwNot,
    Token_KwIs,
    Token_KwInt,
    Token_KwFloat,
    Token_KwBool,
    Token_KwI8,
    Token_KwI16,
    Token_KwI32,
    Token_KwI64,
    Token_KwI128,
    Token_KwISize,
    Token_KwU8,
    Token_KwU16,
    Token_KwU32,
    Token_KwU64,
    Token_KwU128,
    Token_KwUSize,
    Token_KwF32,
    Token_KwF64,
    Token_KwBoolTy,
    Token_KwChar,
    Token_KwStr,
    Token_KwString,
    Token_KwVoid,
    Token_Plus,
    Token_Minus,
    Token_Star,
    Token_Slash,
    Token_Percent,
    Token_StarStar,
    Token_FloorDiv,
    Token_FloorDivEq,
    Token_StarStarEq,
    Token_EqEq,
    Token_NotEq,
    Token_Bang,
    Token_Lt,
    Token_Gt,
    Token_LtEq,
    Token_GtEq,
    Token_Amp,
    Token_Pipe,
    Token_Caret,
    Token_Tilde,
    Token_LtLt,
    Token_GtGt,
    Token_Eq,
    Token_PlusEq,
    Token_MinusEq,
    Token_StarEq,
    Token_SlashEq,
    Token_PercentEq,
    Token_AmpEq,
    Token_PipeEq,
    Token_CaretEq,
    Token_LtLtEq,
    Token_GtGtEq,
    Token_Arrow,
    Token_FatArrow,
    Token_Question,
    Token_At,
    Token_Dot,
    Token_DotDot,
    Token_DotDotEq,
    Token_DotDotDot,
    Token_LParen,
    Token_RParen,
    Token_LBracket,
    Token_RBracket,
    Token_LBrace,
    Token_RBrace,
    Token_Colon,
    Token_Comma,
    Token_Semicolon,
    Token_Hash,
    Token_Indent,
    Token_Dedent,
    Token_Newline,
    Token_Eof,
    Token_Error
} Token_tag;

typedef struct Token {
    Token_tag tag;
    union {
        struct {
            long long val;
        } IntLit;
        struct {
            double val;
        } FloatLit;
        struct {
            TrStr val;
        } StrLit;
        struct {
            TrStr val;
        } TripleStrLit;
        struct {
            TrStr val;
        } ByteStrLit;
        struct {
            TrStr val;
        } RawStrLit;
        struct {
            long long val;
        } CharLit;
        struct {
            TrStr val;
        } FStrLit;
        struct {
            bool val;
        } BoolLit;
        struct {
            TrStr name;
        } Ident;
        struct {
            TrStr msg;
        } Error;
    } data;
} Token;

static inline __attribute__((always_inline)) Token Token_ctor_IntLit(long long val) { Token _r = {.tag=Token_IntLit}; _r.data.IntLit.val = val; return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_FloatLit(double val) { Token _r = {.tag=Token_FloatLit}; _r.data.FloatLit.val = val; return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_StrLit(TrStr val) { Token _r = {.tag=Token_StrLit}; _r.data.StrLit.val = _tr_str_retain(val); return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_TripleStrLit(TrStr val) { Token _r = {.tag=Token_TripleStrLit}; _r.data.TripleStrLit.val = _tr_str_retain(val); return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_ByteStrLit(TrStr val) { Token _r = {.tag=Token_ByteStrLit}; _r.data.ByteStrLit.val = _tr_str_retain(val); return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_RawStrLit(TrStr val) { Token _r = {.tag=Token_RawStrLit}; _r.data.RawStrLit.val = _tr_str_retain(val); return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_CharLit(long long val) { Token _r = {.tag=Token_CharLit}; _r.data.CharLit.val = val; return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_FStrLit(TrStr val) { Token _r = {.tag=Token_FStrLit}; _r.data.FStrLit.val = _tr_str_retain(val); return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_BoolLit(bool val) { Token _r = {.tag=Token_BoolLit}; _r.data.BoolLit.val = val; return _r; }
static inline __attribute__((always_inline)) Token Token_ctor_Ident(TrStr name) { Token _r = {.tag=Token_Ident}; _r.data.Ident.name = _tr_str_retain(name); return _r; }
#define Token_make_KwDef() ((Token){.tag=Token_KwDef})
#define Token_make_KwClass() ((Token){.tag=Token_KwClass})
#define Token_make_KwEnum() ((Token){.tag=Token_KwEnum})
#define Token_make_KwInterface() ((Token){.tag=Token_KwInterface})
#define Token_make_KwExtend() ((Token){.tag=Token_KwExtend})
#define Token_make_KwStruct() ((Token){.tag=Token_KwStruct})
#define Token_make_KwIf() ((Token){.tag=Token_KwIf})
#define Token_make_KwElif() ((Token){.tag=Token_KwElif})
#define Token_make_KwElse() ((Token){.tag=Token_KwElse})
#define Token_make_KwFor() ((Token){.tag=Token_KwFor})
#define Token_make_KwWhile() ((Token){.tag=Token_KwWhile})
#define Token_make_KwLoop() ((Token){.tag=Token_KwLoop})
#define Token_make_KwReturn() ((Token){.tag=Token_KwReturn})
#define Token_make_KwBreak() ((Token){.tag=Token_KwBreak})
#define Token_make_KwContinue() ((Token){.tag=Token_KwContinue})
#define Token_make_KwMatch() ((Token){.tag=Token_KwMatch})
#define Token_make_KwCase() ((Token){.tag=Token_KwCase})
#define Token_make_KwTry() ((Token){.tag=Token_KwTry})
#define Token_make_KwAssert() ((Token){.tag=Token_KwAssert})
#define Token_make_KwWith() ((Token){.tag=Token_KwWith})
#define Token_make_KwDefer() ((Token){.tag=Token_KwDefer})
#define Token_make_KwAsm() ((Token){.tag=Token_KwAsm})
#define Token_make_KwTaskGroup() ((Token){.tag=Token_KwTaskGroup})
#define Token_make_KwGpu() ((Token){.tag=Token_KwGpu})
#define Token_make_KwSizeOf() ((Token){.tag=Token_KwSizeOf})
#define Token_make_KwExcept() ((Token){.tag=Token_KwExcept})
#define Token_make_KwFinally() ((Token){.tag=Token_KwFinally})
#define Token_make_KwRaise() ((Token){.tag=Token_KwRaise})
#define Token_make_KwPass() ((Token){.tag=Token_KwPass})
#define Token_make_KwImplements() ((Token){.tag=Token_KwImplements})
#define Token_make_KwImport() ((Token){.tag=Token_KwImport})
#define Token_make_KwFrom() ((Token){.tag=Token_KwFrom})
#define Token_make_KwAs() ((Token){.tag=Token_KwAs})
#define Token_make_KwIn() ((Token){.tag=Token_KwIn})
#define Token_make_KwMut() ((Token){.tag=Token_KwMut})
#define Token_make_KwShared() ((Token){.tag=Token_KwShared})
#define Token_make_KwExtern() ((Token){.tag=Token_KwExtern})
#define Token_make_KwUnsafe() ((Token){.tag=Token_KwUnsafe})
#define Token_make_KwSpawn() ((Token){.tag=Token_KwSpawn})
#define Token_make_KwAsync() ((Token){.tag=Token_KwAsync})
#define Token_make_KwThrows() ((Token){.tag=Token_KwThrows})
#define Token_make_KwExtends() ((Token){.tag=Token_KwExtends})
#define Token_make_KwAwait() ((Token){.tag=Token_KwAwait})
#define Token_make_KwYield() ((Token){.tag=Token_KwYield})
#define Token_make_KwPub() ((Token){.tag=Token_KwPub})
#define Token_make_KwWhere() ((Token){.tag=Token_KwWhere})
#define Token_make_KwStatic() ((Token){.tag=Token_KwStatic})
#define Token_make_KwStack() ((Token){.tag=Token_KwStack})
#define Token_make_KwOwn() ((Token){.tag=Token_KwOwn})
#define Token_make_KwBorrow() ((Token){.tag=Token_KwBorrow})
#define Token_make_KwMove() ((Token){.tag=Token_KwMove})
#define Token_make_KwConst() ((Token){.tag=Token_KwConst})
#define Token_make_KwActor() ((Token){.tag=Token_KwActor})
#define Token_make_KwSuper() ((Token){.tag=Token_KwSuper})
#define Token_make_KwExport() ((Token){.tag=Token_KwExport})
#define Token_make_KwLambda() ((Token){.tag=Token_KwLambda})
#define Token_make_KwDecorator() ((Token){.tag=Token_KwDecorator})
#define Token_make_KwMacro() ((Token){.tag=Token_KwMacro})
#define Token_make_KwDo() ((Token){.tag=Token_KwDo})
#define Token_make_KwTrue() ((Token){.tag=Token_KwTrue})
#define Token_make_KwFalse() ((Token){.tag=Token_KwFalse})
#define Token_make_KwNone() ((Token){.tag=Token_KwNone})
#define Token_make_KwAnd() ((Token){.tag=Token_KwAnd})
#define Token_make_KwOr() ((Token){.tag=Token_KwOr})
#define Token_make_KwNot() ((Token){.tag=Token_KwNot})
#define Token_make_KwIs() ((Token){.tag=Token_KwIs})
#define Token_make_KwInt() ((Token){.tag=Token_KwInt})
#define Token_make_KwFloat() ((Token){.tag=Token_KwFloat})
#define Token_make_KwBool() ((Token){.tag=Token_KwBool})
#define Token_make_KwI8() ((Token){.tag=Token_KwI8})
#define Token_make_KwI16() ((Token){.tag=Token_KwI16})
#define Token_make_KwI32() ((Token){.tag=Token_KwI32})
#define Token_make_KwI64() ((Token){.tag=Token_KwI64})
#define Token_make_KwI128() ((Token){.tag=Token_KwI128})
#define Token_make_KwISize() ((Token){.tag=Token_KwISize})
#define Token_make_KwU8() ((Token){.tag=Token_KwU8})
#define Token_make_KwU16() ((Token){.tag=Token_KwU16})
#define Token_make_KwU32() ((Token){.tag=Token_KwU32})
#define Token_make_KwU64() ((Token){.tag=Token_KwU64})
#define Token_make_KwU128() ((Token){.tag=Token_KwU128})
#define Token_make_KwUSize() ((Token){.tag=Token_KwUSize})
#define Token_make_KwF32() ((Token){.tag=Token_KwF32})
#define Token_make_KwF64() ((Token){.tag=Token_KwF64})
#define Token_make_KwBoolTy() ((Token){.tag=Token_KwBoolTy})
#define Token_make_KwChar() ((Token){.tag=Token_KwChar})
#define Token_make_KwStr() ((Token){.tag=Token_KwStr})
#define Token_make_KwString() ((Token){.tag=Token_KwString})
#define Token_make_KwVoid() ((Token){.tag=Token_KwVoid})
#define Token_make_Plus() ((Token){.tag=Token_Plus})
#define Token_make_Minus() ((Token){.tag=Token_Minus})
#define Token_make_Star() ((Token){.tag=Token_Star})
#define Token_make_Slash() ((Token){.tag=Token_Slash})
#define Token_make_Percent() ((Token){.tag=Token_Percent})
#define Token_make_StarStar() ((Token){.tag=Token_StarStar})
#define Token_make_FloorDiv() ((Token){.tag=Token_FloorDiv})
#define Token_make_FloorDivEq() ((Token){.tag=Token_FloorDivEq})
#define Token_make_StarStarEq() ((Token){.tag=Token_StarStarEq})
#define Token_make_EqEq() ((Token){.tag=Token_EqEq})
#define Token_make_NotEq() ((Token){.tag=Token_NotEq})
#define Token_make_Bang() ((Token){.tag=Token_Bang})
#define Token_make_Lt() ((Token){.tag=Token_Lt})
#define Token_make_Gt() ((Token){.tag=Token_Gt})
#define Token_make_LtEq() ((Token){.tag=Token_LtEq})
#define Token_make_GtEq() ((Token){.tag=Token_GtEq})
#define Token_make_Amp() ((Token){.tag=Token_Amp})
#define Token_make_Pipe() ((Token){.tag=Token_Pipe})
#define Token_make_Caret() ((Token){.tag=Token_Caret})
#define Token_make_Tilde() ((Token){.tag=Token_Tilde})
#define Token_make_LtLt() ((Token){.tag=Token_LtLt})
#define Token_make_GtGt() ((Token){.tag=Token_GtGt})
#define Token_make_Eq() ((Token){.tag=Token_Eq})
#define Token_make_PlusEq() ((Token){.tag=Token_PlusEq})
#define Token_make_MinusEq() ((Token){.tag=Token_MinusEq})
#define Token_make_StarEq() ((Token){.tag=Token_StarEq})
#define Token_make_SlashEq() ((Token){.tag=Token_SlashEq})
#define Token_make_PercentEq() ((Token){.tag=Token_PercentEq})
#define Token_make_AmpEq() ((Token){.tag=Token_AmpEq})
#define Token_make_PipeEq() ((Token){.tag=Token_PipeEq})
#define Token_make_CaretEq() ((Token){.tag=Token_CaretEq})
#define Token_make_LtLtEq() ((Token){.tag=Token_LtLtEq})
#define Token_make_GtGtEq() ((Token){.tag=Token_GtGtEq})
#define Token_make_Arrow() ((Token){.tag=Token_Arrow})
#define Token_make_FatArrow() ((Token){.tag=Token_FatArrow})
#define Token_make_Question() ((Token){.tag=Token_Question})
#define Token_make_At() ((Token){.tag=Token_At})
#define Token_make_Dot() ((Token){.tag=Token_Dot})
#define Token_make_DotDot() ((Token){.tag=Token_DotDot})
#define Token_make_DotDotEq() ((Token){.tag=Token_DotDotEq})
#define Token_make_DotDotDot() ((Token){.tag=Token_DotDotDot})
#define Token_make_LParen() ((Token){.tag=Token_LParen})
#define Token_make_RParen() ((Token){.tag=Token_RParen})
#define Token_make_LBracket() ((Token){.tag=Token_LBracket})
#define Token_make_RBracket() ((Token){.tag=Token_RBracket})
#define Token_make_LBrace() ((Token){.tag=Token_LBrace})
#define Token_make_RBrace() ((Token){.tag=Token_RBrace})
#define Token_make_Colon() ((Token){.tag=Token_Colon})
#define Token_make_Comma() ((Token){.tag=Token_Comma})
#define Token_make_Semicolon() ((Token){.tag=Token_Semicolon})
#define Token_make_Hash() ((Token){.tag=Token_Hash})
#define Token_make_Indent() ((Token){.tag=Token_Indent})
#define Token_make_Dedent() ((Token){.tag=Token_Dedent})
#define Token_make_Newline() ((Token){.tag=Token_Newline})
#define Token_make_Eof() ((Token){.tag=Token_Eof})
static inline __attribute__((always_inline)) Token Token_ctor_Error(TrStr msg) { Token _r = {.tag=Token_Error}; _r.data.Error.msg = _tr_str_retain(msg); return _r; }

typedef enum {
    Pattern_PWild,
    Pattern_PBind,
    Pattern_PLitInt,
    Pattern_PLitStr,
    Pattern_PLitBool,
    Pattern_PVariant,
    Pattern_PVariantBind,
    Pattern_PVariantBindMany,
    Pattern_PTuple,
    Pattern_POr
} Pattern_tag;

typedef struct Pattern {
    Pattern_tag tag;
    union {
        struct {
            TrStr name;
        } PBind;
        struct {
            long long val;
        } PLitInt;
        struct {
            TrStr val;
        } PLitStr;
        struct {
            bool val;
        } PLitBool;
        struct {
            TrStr type_name;
            TrStr variant;
        } PVariant;
        struct {
            TrStr type_name;
            TrStr variant;
            TrStr field;
        } PVariantBind;
        struct {
            TrStr type_name;
            TrStr variant;
            List_TrStr* fields;
        } PVariantBindMany;
        struct {
            TrStr first;
            TrStr second;
        } PTuple;
        struct {
            List_Pattern* patterns;
        } POr;
    } data;
} Pattern;

#define Pattern_make_PWild() ((Pattern){.tag=Pattern_PWild})
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PBind(TrStr name) { Pattern _r = {.tag=Pattern_PBind}; _r.data.PBind.name = _tr_str_retain(name); return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PLitInt(long long val) { Pattern _r = {.tag=Pattern_PLitInt}; _r.data.PLitInt.val = val; return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PLitStr(TrStr val) { Pattern _r = {.tag=Pattern_PLitStr}; _r.data.PLitStr.val = _tr_str_retain(val); return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PLitBool(bool val) { Pattern _r = {.tag=Pattern_PLitBool}; _r.data.PLitBool.val = val; return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PVariant(TrStr type_name, TrStr variant) { Pattern _r = {.tag=Pattern_PVariant}; _r.data.PVariant.type_name = _tr_str_retain(type_name); _r.data.PVariant.variant = _tr_str_retain(variant); return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PVariantBind(TrStr type_name, TrStr variant, TrStr field) { Pattern _r = {.tag=Pattern_PVariantBind}; _r.data.PVariantBind.type_name = _tr_str_retain(type_name); _r.data.PVariantBind.variant = _tr_str_retain(variant); _r.data.PVariantBind.field = _tr_str_retain(field); return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PVariantBindMany(TrStr type_name, TrStr variant, List_TrStr* fields) { Pattern _r = {.tag=Pattern_PVariantBindMany}; _r.data.PVariantBindMany.type_name = _tr_str_retain(type_name); _r.data.PVariantBindMany.variant = _tr_str_retain(variant); _r.data.PVariantBindMany.fields = fields; return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_PTuple(TrStr first, TrStr second) { Pattern _r = {.tag=Pattern_PTuple}; _r.data.PTuple.first = _tr_str_retain(first); _r.data.PTuple.second = _tr_str_retain(second); return _r; }
static inline __attribute__((always_inline)) Pattern Pattern_ctor_POr(List_Pattern* patterns) { Pattern _r = {.tag=Pattern_POr}; _r.data.POr.patterns = patterns; return _r; }

typedef enum {
    Ownership_Own,
    Ownership_Shared
} Ownership_tag;

typedef struct Ownership {
    Ownership_tag tag;
} Ownership;

#define Ownership_make_Own() ((Ownership){.tag=Ownership_Own})
#define Ownership_make_Shared() ((Ownership){.tag=Ownership_Shared})

typedef enum {
    Expr_ELitInt,
    Expr_ELitFloat,
    Expr_ELitStr,
    Expr_ELitBool,
    Expr_ELitChar,
    Expr_ELitBytes,
    Expr_ERawStr,
    Expr_ELitNone,
    Expr_EIdent,
    Expr_EBinOp,
    Expr_EUnaryOp,
    Expr_ECall,
    Expr_EMethodCall,
    Expr_EPropAccess,
    Expr_EIndex,
    Expr_ECast,
    Expr_EFString,
    Expr_ETryExpr,
    Expr_EClosure,
    Expr_ESuperMethodCall,
    Expr_ESuperPropAccess,
    Expr_EList,
    Expr_ESet,
    Expr_EDict,
    Expr_ETuple,
    Expr_EListComp,
    Expr_EGeneratorExpr,
    Expr_ESlice,
    Expr_EAwait,
    Expr_EYield,
    Expr_ETry,
    Expr_ERange,
    Expr_ESizeOf,
    Expr_EIfElse,
    Expr_ETypeArg,
    Expr_EDo,
    Expr_EMatch,
    Expr_EMacroCall,
    Expr_ELoop,
    Expr_EWhileExpr
} Expr_tag;

typedef struct Expr {
    Expr_tag tag;
    union {
        struct {
            long long val;
        } ELitInt;
        struct {
            double val;
        } ELitFloat;
        struct {
            TrStr val;
        } ELitStr;
        struct {
            bool val;
        } ELitBool;
        struct {
            long long val;
        } ELitChar;
        struct {
            TrStr val;
        } ELitBytes;
        struct {
            TrStr val;
        } ERawStr;
        struct {
            TrStr name;
        } EIdent;
        struct {
            TrStr op;
            Expr* left;
            Expr* right;
        } EBinOp;
        struct {
            TrStr op;
            Expr* expr;
        } EUnaryOp;
        struct {
            Expr* callee;
            List_ptr* args;
        } ECall;
        struct {
            Expr* obj;
            TrStr method;
            List_ptr* args;
        } EMethodCall;
        struct {
            Expr* obj;
            TrStr prop;
        } EPropAccess;
        struct {
            Expr* obj;
            Expr* _tr_v_index;
        } EIndex;
        struct {
            Expr* expr;
            AstType** ty;
        } ECast;
        struct {
            List_ptr* parts;
        } EFString;
        struct {
            Expr* expr;
        } ETryExpr;
        struct {
            List_ptr* params;
            AstType** ret_ty;
            Block* body;
            bool is_async;
        } EClosure;
        struct {
            TrStr base_class;
            TrStr method;
            List_ptr* args;
        } ESuperMethodCall;
        struct {
            TrStr base_class;
            TrStr prop;
        } ESuperPropAccess;
        struct {
            List_ptr* items;
        } EList;
        struct {
            List_ptr* items;
        } ESet;
        struct {
            List_ptr* keys;
            List_ptr* vals;
        } EDict;
        struct {
            List_ptr* items;
        } ETuple;
        struct {
            Expr* element;
            List_ptr* generators;
        } EListComp;
        struct {
            Expr* element;
            List_ptr* generators;
        } EGeneratorExpr;
        struct {
            Expr* start;
            Expr* stop;
            Expr* step;
        } ESlice;
        struct {
            Expr* expr;
        } EAwait;
        struct {
            Expr* expr;
        } EYield;
        struct {
            Block* try_body;
            List_ptr* catches;
            Block* finally_b;
        } ETry;
        struct {
            Expr* start;
            Expr* end;
            bool inclusive;
        } ERange;
        struct {
            AstType** ty;
        } ESizeOf;
        struct {
            Expr* cond;
            Expr* then_expr;
            Expr* else_expr;
        } EIfElse;
        struct {
            AstType** ty;
        } ETypeArg;
        struct {
            Block* body;
        } EDo;
        struct {
            Expr* subj;
            List_ptr* arms;
        } EMatch;
        struct {
            TrStr name;
            List_ptr* args;
        } EMacroCall;
        struct {
            Block* body;
        } ELoop;
        struct {
            Expr* cond;
            Block* body;
            Block* else_body;
        } EWhileExpr;
    } data;
} Expr;

static inline __attribute__((always_inline)) Expr Expr_ctor_ELitInt(long long val) { Expr _r = {.tag=Expr_ELitInt}; _r.data.ELitInt.val = val; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ELitFloat(double val) { Expr _r = {.tag=Expr_ELitFloat}; _r.data.ELitFloat.val = val; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ELitStr(TrStr val) { Expr _r = {.tag=Expr_ELitStr}; _r.data.ELitStr.val = _tr_str_retain(val); return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ELitBool(bool val) { Expr _r = {.tag=Expr_ELitBool}; _r.data.ELitBool.val = val; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ELitChar(long long val) { Expr _r = {.tag=Expr_ELitChar}; _r.data.ELitChar.val = val; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ELitBytes(TrStr val) { Expr _r = {.tag=Expr_ELitBytes}; _r.data.ELitBytes.val = _tr_str_retain(val); return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ERawStr(TrStr val) { Expr _r = {.tag=Expr_ERawStr}; _r.data.ERawStr.val = _tr_str_retain(val); return _r; }
#define Expr_make_ELitNone() ((Expr){.tag=Expr_ELitNone})
static inline __attribute__((always_inline)) Expr Expr_ctor_EIdent(TrStr name) { Expr _r = {.tag=Expr_EIdent}; _r.data.EIdent.name = _tr_str_retain(name); return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EBinOp(TrStr op, Expr* left, Expr* right) { Expr _r = {.tag=Expr_EBinOp}; _r.data.EBinOp.op = _tr_str_retain(op); _r.data.EBinOp.left = left; _r.data.EBinOp.right = right; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EUnaryOp(TrStr op, Expr* expr) { Expr _r = {.tag=Expr_EUnaryOp}; _r.data.EUnaryOp.op = _tr_str_retain(op); _r.data.EUnaryOp.expr = expr; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ECall(Expr* callee, List_ptr* args) { Expr _r = {.tag=Expr_ECall}; _r.data.ECall.callee = callee; _r.data.ECall.args = args; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EMethodCall(Expr* obj, TrStr method, List_ptr* args) { Expr _r = {.tag=Expr_EMethodCall}; _r.data.EMethodCall.obj = obj; _r.data.EMethodCall.method = _tr_str_retain(method); _r.data.EMethodCall.args = args; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EPropAccess(Expr* obj, TrStr prop) { Expr _r = {.tag=Expr_EPropAccess}; _r.data.EPropAccess.obj = obj; _r.data.EPropAccess.prop = _tr_str_retain(prop); return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EIndex(Expr* obj, Expr* _tr_v_index) { Expr _r = {.tag=Expr_EIndex}; _r.data.EIndex.obj = obj; _r.data.EIndex._tr_v_index = _tr_v_index; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ECast(Expr* expr, AstType** ty) { Expr _r = {.tag=Expr_ECast}; _r.data.ECast.expr = expr; _r.data.ECast.ty = ty; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EFString(List_ptr* parts) { Expr _r = {.tag=Expr_EFString}; _r.data.EFString.parts = parts; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ETryExpr(Expr* expr) { Expr _r = {.tag=Expr_ETryExpr}; _r.data.ETryExpr.expr = expr; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EClosure(List_ptr* params, AstType** ret_ty, Block* body, bool is_async) { Expr _r = {.tag=Expr_EClosure}; _r.data.EClosure.params = params; _r.data.EClosure.ret_ty = ret_ty; _r.data.EClosure.body = body; _r.data.EClosure.is_async = is_async; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ESuperMethodCall(TrStr base_class, TrStr method, List_ptr* args) { Expr _r = {.tag=Expr_ESuperMethodCall}; _r.data.ESuperMethodCall.base_class = _tr_str_retain(base_class); _r.data.ESuperMethodCall.method = _tr_str_retain(method); _r.data.ESuperMethodCall.args = args; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ESuperPropAccess(TrStr base_class, TrStr prop) { Expr _r = {.tag=Expr_ESuperPropAccess}; _r.data.ESuperPropAccess.base_class = _tr_str_retain(base_class); _r.data.ESuperPropAccess.prop = _tr_str_retain(prop); return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EList(List_ptr* items) { Expr _r = {.tag=Expr_EList}; _r.data.EList.items = items; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ESet(List_ptr* items) { Expr _r = {.tag=Expr_ESet}; _r.data.ESet.items = items; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EDict(List_ptr* keys, List_ptr* vals) { Expr _r = {.tag=Expr_EDict}; _r.data.EDict.keys = keys; _r.data.EDict.vals = vals; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ETuple(List_ptr* items) { Expr _r = {.tag=Expr_ETuple}; _r.data.ETuple.items = items; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EListComp(Expr* element, List_ptr* generators) { Expr _r = {.tag=Expr_EListComp}; _r.data.EListComp.element = element; _r.data.EListComp.generators = generators; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EGeneratorExpr(Expr* element, List_ptr* generators) { Expr _r = {.tag=Expr_EGeneratorExpr}; _r.data.EGeneratorExpr.element = element; _r.data.EGeneratorExpr.generators = generators; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ESlice(Expr* start, Expr* stop, Expr* step) { Expr _r = {.tag=Expr_ESlice}; _r.data.ESlice.start = start; _r.data.ESlice.stop = stop; _r.data.ESlice.step = step; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EAwait(Expr* expr) { Expr _r = {.tag=Expr_EAwait}; _r.data.EAwait.expr = expr; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EYield(Expr* expr) { Expr _r = {.tag=Expr_EYield}; _r.data.EYield.expr = expr; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ETry(Block* try_body, List_ptr* catches, Block* finally_b) { Expr _r = {.tag=Expr_ETry}; _r.data.ETry.try_body = try_body; _r.data.ETry.catches = catches; _r.data.ETry.finally_b = finally_b; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ERange(Expr* start, Expr* end, bool inclusive) { Expr _r = {.tag=Expr_ERange}; _r.data.ERange.start = start; _r.data.ERange.end = end; _r.data.ERange.inclusive = inclusive; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ESizeOf(AstType** ty) { Expr _r = {.tag=Expr_ESizeOf}; _r.data.ESizeOf.ty = ty; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EIfElse(Expr* cond, Expr* then_expr, Expr* else_expr) { Expr _r = {.tag=Expr_EIfElse}; _r.data.EIfElse.cond = cond; _r.data.EIfElse.then_expr = then_expr; _r.data.EIfElse.else_expr = else_expr; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ETypeArg(AstType** ty) { Expr _r = {.tag=Expr_ETypeArg}; _r.data.ETypeArg.ty = ty; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EDo(Block* body) { Expr _r = {.tag=Expr_EDo}; _r.data.EDo.body = body; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EMatch(Expr* subj, List_ptr* arms) { Expr _r = {.tag=Expr_EMatch}; _r.data.EMatch.subj = subj; _r.data.EMatch.arms = arms; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EMacroCall(TrStr name, List_ptr* args) { Expr _r = {.tag=Expr_EMacroCall}; _r.data.EMacroCall.name = _tr_str_retain(name); _r.data.EMacroCall.args = args; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_ELoop(Block* body) { Expr _r = {.tag=Expr_ELoop}; _r.data.ELoop.body = body; return _r; }
static inline __attribute__((always_inline)) Expr Expr_ctor_EWhileExpr(Expr* cond, Block* body, Block* else_body) { Expr _r = {.tag=Expr_EWhileExpr}; _r.data.EWhileExpr.cond = cond; _r.data.EWhileExpr.body = body; _r.data.EWhileExpr.else_body = else_body; return _r; }

typedef enum {
    Stmt_SExpr,
    Stmt_SLet,
    Stmt_SMultiLet,
    Stmt_SAssign,
    Stmt_SReturn,
    Stmt_SBreak,
    Stmt_SContinue,
    Stmt_SPass,
    Stmt_SRaise,
    Stmt_SUnsafe,
    Stmt_SIf,
    Stmt_SWhile,
    Stmt_SFor,
    Stmt_SForUnpack,
    Stmt_SMatch,
    Stmt_STry,
    Stmt_SAssert,
    Stmt_SWith,
    Stmt_SAsm,
    Stmt_SSpawn,
    Stmt_STaskGroup,
    Stmt_SGpuBlock,
    Stmt_SChanSelect,
    Stmt_SDefer,
    Stmt_SLine,
    Stmt_SLocalDecl
} Stmt_tag;

typedef struct Stmt {
    Stmt_tag tag;
    union {
        struct {
            Expr* expr;
        } SExpr;
        struct {
            TrStr name;
            Ownership ownership;
            bool is_mut;
            bool is_const;
            bool is_shared;
            AstType** ty;
            Expr* val;
        } SLet;
        struct {
            List_TrStr* names;
            bool is_mut;
            Expr* val;
        } SMultiLet;
        struct {
            Expr* target;
            Expr* val;
        } SAssign;
        struct {
            Expr* val;
        } SReturn;
        struct {
            Expr* val;
        } SBreak;
        struct {
            Expr* val;
        } SRaise;
        struct {
            Block* body;
        } SUnsafe;
        struct {
            Expr* cond;
            Block* then_b;
            List_ptr* elifs;
            Block* else_b;
        } SIf;
        struct {
            Expr* cond;
            Block* body;
            List_ptr* decorators;
        } SWhile;
        struct {
            TrStr var;
            Expr* iter;
            Block* body;
            List_ptr* decorators;
            bool is_ref;
        } SFor;
        struct {
            List_TrStr* vars;
            Expr* iter;
            Block* body;
        } SForUnpack;
        struct {
            Expr* expr;
            List_ptr* arms;
        } SMatch;
        struct {
            Block* try_body;
            List_ptr* catches;
            Block* finally_b;
        } STry;
        struct {
            Expr* cond;
            Expr* msg;
        } SAssert;
        struct {
            List_ptr* items;
            List_TrStr* aliases;
            Block* body;
        } SWith;
        struct {
            TrStr code;
            TrStr outputs;
            TrStr inputs;
            TrStr clobbers;
        } SAsm;
        struct {
            Expr* expr;
        } SSpawn;
        struct {
            Block* body;
        } STaskGroup;
        struct {
            Block* body;
        } SGpuBlock;
        struct {
            List_ptr* cases;
        } SChanSelect;
        struct {
            Stmt* stmt;
        } SDefer;
        struct {
            long long n;
        } SLine;
        struct {
            Decl* decl;
        } SLocalDecl;
    } data;
} Stmt;

static inline __attribute__((always_inline)) Stmt Stmt_ctor_SExpr(Expr* expr) { Stmt _r = {.tag=Stmt_SExpr}; _r.data.SExpr.expr = expr; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SLet(TrStr name, Ownership ownership, bool is_mut, bool is_const, bool is_shared, AstType** ty, Expr* val) { Stmt _r = {.tag=Stmt_SLet}; _r.data.SLet.name = _tr_str_retain(name); _r.data.SLet.ownership = ownership; _r.data.SLet.is_mut = is_mut; _r.data.SLet.is_const = is_const; _r.data.SLet.is_shared = is_shared; _r.data.SLet.ty = ty; _r.data.SLet.val = val; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SMultiLet(List_TrStr* names, bool is_mut, Expr* val) { Stmt _r = {.tag=Stmt_SMultiLet}; _r.data.SMultiLet.names = names; _r.data.SMultiLet.is_mut = is_mut; _r.data.SMultiLet.val = val; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SAssign(Expr* target, Expr* val) { Stmt _r = {.tag=Stmt_SAssign}; _r.data.SAssign.target = target; _r.data.SAssign.val = val; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SReturn(Expr* val) { Stmt _r = {.tag=Stmt_SReturn}; _r.data.SReturn.val = val; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SBreak(Expr* val) { Stmt _r = {.tag=Stmt_SBreak}; _r.data.SBreak.val = val; return _r; }
#define Stmt_make_SContinue() ((Stmt){.tag=Stmt_SContinue})
#define Stmt_make_SPass() ((Stmt){.tag=Stmt_SPass})
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SRaise(Expr* val) { Stmt _r = {.tag=Stmt_SRaise}; _r.data.SRaise.val = val; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SUnsafe(Block* body) { Stmt _r = {.tag=Stmt_SUnsafe}; _r.data.SUnsafe.body = body; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SIf(Expr* cond, Block* then_b, List_ptr* elifs, Block* else_b) { Stmt _r = {.tag=Stmt_SIf}; _r.data.SIf.cond = cond; _r.data.SIf.then_b = then_b; _r.data.SIf.elifs = elifs; _r.data.SIf.else_b = else_b; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SWhile(Expr* cond, Block* body, List_ptr* decorators) { Stmt _r = {.tag=Stmt_SWhile}; _r.data.SWhile.cond = cond; _r.data.SWhile.body = body; _r.data.SWhile.decorators = decorators; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SFor(TrStr var, Expr* iter, Block* body, List_ptr* decorators, bool is_ref) { Stmt _r = {.tag=Stmt_SFor}; _r.data.SFor.var = _tr_str_retain(var); _r.data.SFor.iter = iter; _r.data.SFor.body = body; _r.data.SFor.decorators = decorators; _r.data.SFor.is_ref = is_ref; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SForUnpack(List_TrStr* vars, Expr* iter, Block* body) { Stmt _r = {.tag=Stmt_SForUnpack}; _r.data.SForUnpack.vars = vars; _r.data.SForUnpack.iter = iter; _r.data.SForUnpack.body = body; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SMatch(Expr* expr, List_ptr* arms) { Stmt _r = {.tag=Stmt_SMatch}; _r.data.SMatch.expr = expr; _r.data.SMatch.arms = arms; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_STry(Block* try_body, List_ptr* catches, Block* finally_b) { Stmt _r = {.tag=Stmt_STry}; _r.data.STry.try_body = try_body; _r.data.STry.catches = catches; _r.data.STry.finally_b = finally_b; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SAssert(Expr* cond, Expr* msg) { Stmt _r = {.tag=Stmt_SAssert}; _r.data.SAssert.cond = cond; _r.data.SAssert.msg = msg; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SWith(List_ptr* items, List_TrStr* aliases, Block* body) { Stmt _r = {.tag=Stmt_SWith}; _r.data.SWith.items = items; _r.data.SWith.aliases = aliases; _r.data.SWith.body = body; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SAsm(TrStr code, TrStr outputs, TrStr inputs, TrStr clobbers) { Stmt _r = {.tag=Stmt_SAsm}; _r.data.SAsm.code = _tr_str_retain(code); _r.data.SAsm.outputs = _tr_str_retain(outputs); _r.data.SAsm.inputs = _tr_str_retain(inputs); _r.data.SAsm.clobbers = _tr_str_retain(clobbers); return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SSpawn(Expr* expr) { Stmt _r = {.tag=Stmt_SSpawn}; _r.data.SSpawn.expr = expr; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_STaskGroup(Block* body) { Stmt _r = {.tag=Stmt_STaskGroup}; _r.data.STaskGroup.body = body; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SGpuBlock(Block* body) { Stmt _r = {.tag=Stmt_SGpuBlock}; _r.data.SGpuBlock.body = body; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SChanSelect(List_ptr* cases) { Stmt _r = {.tag=Stmt_SChanSelect}; _r.data.SChanSelect.cases = cases; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SDefer(Stmt* stmt) { Stmt _r = {.tag=Stmt_SDefer}; _r.data.SDefer.stmt = stmt; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SLine(long long n) { Stmt _r = {.tag=Stmt_SLine}; _r.data.SLine.n = n; return _r; }
static inline __attribute__((always_inline)) Stmt Stmt_ctor_SLocalDecl(Decl* decl) { Stmt _r = {.tag=Stmt_SLocalDecl}; _r.data.SLocalDecl.decl = decl; return _r; }

typedef enum {
    Decl_DFunction,
    Decl_DClass,
    Decl_DEnum,
    Decl_DInterface,
    Decl_DExtend,
    Decl_DImport,
    Decl_DFromImport,
    Decl_DExtern,
    Decl_DTopLevelStmt,
    Decl_DActor,
    Decl_DDecoratorDef,
    Decl_DTypeAlias
} Decl_tag;

typedef struct Decl {
    Decl_tag tag;
    union {
        struct {
            FunctionDef* func;
        } DFunction;
        struct {
            ClassDef* cls;
        } DClass;
        struct {
            EnumDef* enm;
        } DEnum;
        struct {
            InterfaceDef* iface;
        } DInterface;
        struct {
            TrStr target;
            List_ptr* methods;
        } DExtend;
        struct {
            TrStr path;
            TrStr alias;
        } DImport;
        struct {
            TrStr path;
            List_ptr* items;
        } DFromImport;
        struct {
            TrStr abi;
            List_ptr* functions;
        } DExtern;
        struct {
            Stmt* stmt;
        } DTopLevelStmt;
        struct {
            ClassDef* cls;
        } DActor;
        struct {
            FunctionDef* func;
        } DDecoratorDef;
        struct {
            TrStr name;
            AstType** target;
        } DTypeAlias;
    } data;
} Decl;

static inline __attribute__((always_inline)) Decl Decl_ctor_DFunction(FunctionDef* func) { Decl _r = {.tag=Decl_DFunction}; _r.data.DFunction.func = _tr_obj_retain(func); return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DClass(ClassDef* cls) { Decl _r = {.tag=Decl_DClass}; _r.data.DClass.cls = _tr_obj_retain(cls); return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DEnum(EnumDef* enm) { Decl _r = {.tag=Decl_DEnum}; _r.data.DEnum.enm = _tr_obj_retain(enm); return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DInterface(InterfaceDef* iface) { Decl _r = {.tag=Decl_DInterface}; _r.data.DInterface.iface = _tr_obj_retain(iface); return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DExtend(TrStr target, List_ptr* methods) { Decl _r = {.tag=Decl_DExtend}; _r.data.DExtend.target = _tr_str_retain(target); _r.data.DExtend.methods = methods; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DImport(TrStr path, TrStr alias) { Decl _r = {.tag=Decl_DImport}; _r.data.DImport.path = _tr_str_retain(path); _r.data.DImport.alias = _tr_str_retain(alias); return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DFromImport(TrStr path, List_ptr* items) { Decl _r = {.tag=Decl_DFromImport}; _r.data.DFromImport.path = _tr_str_retain(path); _r.data.DFromImport.items = items; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DExtern(TrStr abi, List_ptr* functions) { Decl _r = {.tag=Decl_DExtern}; _r.data.DExtern.abi = _tr_str_retain(abi); _r.data.DExtern.functions = functions; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DTopLevelStmt(Stmt* stmt) { Decl _r = {.tag=Decl_DTopLevelStmt}; _r.data.DTopLevelStmt.stmt = stmt; return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DActor(ClassDef* cls) { Decl _r = {.tag=Decl_DActor}; _r.data.DActor.cls = _tr_obj_retain(cls); return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DDecoratorDef(FunctionDef* func) { Decl _r = {.tag=Decl_DDecoratorDef}; _r.data.DDecoratorDef.func = _tr_obj_retain(func); return _r; }
static inline __attribute__((always_inline)) Decl Decl_ctor_DTypeAlias(TrStr name, AstType** target) { Decl _r = {.tag=Decl_DTypeAlias}; _r.data.DTypeAlias.name = _tr_str_retain(name); _r.data.DTypeAlias.target = target; return _r; }

typedef enum {
    HirExpr_ELitInt,
    HirExpr_ELitFloat,
    HirExpr_ELitStr,
    HirExpr_ELitBytes,
    HirExpr_ERawStr,
    HirExpr_ELitChar,
    HirExpr_ELitBool,
    HirExpr_ELitNone,
    HirExpr_EIdent,
    HirExpr_EBinOp,
    HirExpr_EUnaryOp,
    HirExpr_ECall,
    HirExpr_EMethodCall,
    HirExpr_EPropAccess,
    HirExpr_EIndex,
    HirExpr_ECast,
    HirExpr_EFString,
    HirExpr_ETryExpr,
    HirExpr_EClosure,
    HirExpr_ESuperMethodCall,
    HirExpr_ESuperPropAccess,
    HirExpr_EList,
    HirExpr_ESet,
    HirExpr_EDict,
    HirExpr_ETuple,
    HirExpr_EListComp,
    HirExpr_EGeneratorExpr,
    HirExpr_ESlice,
    HirExpr_EAwait,
    HirExpr_EAwaitTimeout,
    HirExpr_EYield,
    HirExpr_ETry,
    HirExpr_ERange,
    HirExpr_ESizeOf,
    HirExpr_EIfElse,
    HirExpr_EDo,
    HirExpr_EMatchExpr,
    HirExpr_ELoop,
    HirExpr_EWhileExpr
} HirExpr_tag;

typedef struct HirExpr {
    HirExpr_tag tag;
    union {
        struct {
            long long val;
            AstType* ty;
        } ELitInt;
        struct {
            double val;
            AstType* ty;
        } ELitFloat;
        struct {
            TrStr val;
            AstType* ty;
        } ELitStr;
        struct {
            TrStr val;
            AstType* ty;
        } ELitBytes;
        struct {
            TrStr val;
            AstType* ty;
        } ERawStr;
        struct {
            long long val;
            AstType* ty;
        } ELitChar;
        struct {
            bool val;
            AstType* ty;
        } ELitBool;
        struct {
            AstType* ty;
        } ELitNone;
        struct {
            TrStr name;
            AstType* ty;
            bool is_move;
        } EIdent;
        struct {
            TrStr op;
            HirExpr* left;
            HirExpr* right;
            AstType* ty;
        } EBinOp;
        struct {
            TrStr op;
            HirExpr* expr;
            AstType* ty;
        } EUnaryOp;
        struct {
            HirExpr* callee;
            List_ptr* args;
            AstType* ty;
        } ECall;
        struct {
            HirExpr* obj;
            TrStr method;
            List_ptr* args;
            AstType* ty;
        } EMethodCall;
        struct {
            HirExpr* obj;
            TrStr prop;
            AstType* ty;
        } EPropAccess;
        struct {
            HirExpr* obj;
            HirExpr* _tr_v_index;
            AstType* ty;
        } EIndex;
        struct {
            HirExpr* expr;
            AstType* target_ty;
        } ECast;
        struct {
            List_ptr* parts;
            AstType* ty;
        } EFString;
        struct {
            HirExpr* expr;
            AstType* ty;
        } ETryExpr;
        struct {
            List_ptr* params;
            AstType* ret_ty;
            HirBlock* body;
            bool is_async;
            List_ptr* captures;
        } EClosure;
        struct {
            TrStr base_class;
            TrStr method;
            List_ptr* args;
            AstType* ty;
        } ESuperMethodCall;
        struct {
            TrStr base_class;
            TrStr prop;
            AstType* ty;
        } ESuperPropAccess;
        struct {
            List_ptr* items;
            AstType* ty;
        } EList;
        struct {
            List_ptr* items;
            AstType* ty;
        } ESet;
        struct {
            List_ptr* keys;
            List_ptr* vals;
            AstType* ty;
        } EDict;
        struct {
            List_ptr* items;
            AstType* ty;
        } ETuple;
        struct {
            HirExpr* element;
            List_ptr* generators;
            AstType* ty;
        } EListComp;
        struct {
            HirExpr* element;
            List_ptr* generators;
            AstType* ty;
        } EGeneratorExpr;
        struct {
            HirExpr* start;
            HirExpr* stop;
            HirExpr* step;
            AstType* ty;
        } ESlice;
        struct {
            HirExpr* expr;
            AstType* ty;
        } EAwait;
        struct {
            HirExpr* expr;
            HirExpr* timeout_ms;
            AstType* ty;
        } EAwaitTimeout;
        struct {
            HirExpr* expr;
            AstType* ty;
        } EYield;
        struct {
            HirBlock* try_body;
            List_ptr* catches;
            HirBlock* finally_b;
            AstType* ty;
        } ETry;
        struct {
            HirExpr* start;
            HirExpr* end;
            bool inclusive;
            AstType* ty;
        } ERange;
        struct {
            AstType* target_ty;
            AstType* ty;
        } ESizeOf;
        struct {
            HirExpr* cond;
            HirExpr* then_e;
            HirExpr* else_e;
            AstType* ty;
        } EIfElse;
        struct {
            HirBlock* body;
            AstType* ty;
        } EDo;
        struct {
            HirExpr* subj;
            List_ptr* arms;
            AstType* ty;
        } EMatchExpr;
        struct {
            HirBlock* body;
            AstType* ty;
        } ELoop;
        struct {
            HirExpr* cond;
            HirBlock* body;
            HirBlock* else_body;
            AstType* ty;
        } EWhileExpr;
    } data;
} HirExpr;

static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitInt(long long val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitInt}; _r.data.ELitInt.val = val; _r.data.ELitInt.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitFloat(double val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitFloat}; _r.data.ELitFloat.val = val; _r.data.ELitFloat.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitStr(TrStr val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitStr}; _r.data.ELitStr.val = _tr_str_retain(val); _r.data.ELitStr.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitBytes(TrStr val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitBytes}; _r.data.ELitBytes.val = _tr_str_retain(val); _r.data.ELitBytes.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ERawStr(TrStr val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ERawStr}; _r.data.ERawStr.val = _tr_str_retain(val); _r.data.ERawStr.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitChar(long long val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitChar}; _r.data.ELitChar.val = val; _r.data.ELitChar.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitBool(bool val, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitBool}; _r.data.ELitBool.val = val; _r.data.ELitBool.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELitNone(AstType* ty) { HirExpr _r = {.tag=HirExpr_ELitNone}; _r.data.ELitNone.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EIdent(TrStr name, AstType* ty, bool is_move) { HirExpr _r = {.tag=HirExpr_EIdent}; _r.data.EIdent.name = _tr_str_retain(name); _r.data.EIdent.ty = ty; _r.data.EIdent.is_move = is_move; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EBinOp(TrStr op, HirExpr* left, HirExpr* right, AstType* ty) { HirExpr _r = {.tag=HirExpr_EBinOp}; _r.data.EBinOp.op = _tr_str_retain(op); _r.data.EBinOp.left = left; _r.data.EBinOp.right = right; _r.data.EBinOp.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EUnaryOp(TrStr op, HirExpr* expr, AstType* ty) { HirExpr _r = {.tag=HirExpr_EUnaryOp}; _r.data.EUnaryOp.op = _tr_str_retain(op); _r.data.EUnaryOp.expr = expr; _r.data.EUnaryOp.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ECall(HirExpr* callee, List_ptr* args, AstType* ty) { HirExpr _r = {.tag=HirExpr_ECall}; _r.data.ECall.callee = callee; _r.data.ECall.args = args; _r.data.ECall.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EMethodCall(HirExpr* obj, TrStr method, List_ptr* args, AstType* ty) { HirExpr _r = {.tag=HirExpr_EMethodCall}; _r.data.EMethodCall.obj = obj; _r.data.EMethodCall.method = _tr_str_retain(method); _r.data.EMethodCall.args = args; _r.data.EMethodCall.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EPropAccess(HirExpr* obj, TrStr prop, AstType* ty) { HirExpr _r = {.tag=HirExpr_EPropAccess}; _r.data.EPropAccess.obj = obj; _r.data.EPropAccess.prop = _tr_str_retain(prop); _r.data.EPropAccess.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EIndex(HirExpr* obj, HirExpr* _tr_v_index, AstType* ty) { HirExpr _r = {.tag=HirExpr_EIndex}; _r.data.EIndex.obj = obj; _r.data.EIndex._tr_v_index = _tr_v_index; _r.data.EIndex.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ECast(HirExpr* expr, AstType* target_ty) { HirExpr _r = {.tag=HirExpr_ECast}; _r.data.ECast.expr = expr; _r.data.ECast.target_ty = target_ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EFString(List_ptr* parts, AstType* ty) { HirExpr _r = {.tag=HirExpr_EFString}; _r.data.EFString.parts = parts; _r.data.EFString.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ETryExpr(HirExpr* expr, AstType* ty) { HirExpr _r = {.tag=HirExpr_ETryExpr}; _r.data.ETryExpr.expr = expr; _r.data.ETryExpr.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EClosure(List_ptr* params, AstType* ret_ty, HirBlock* body, bool is_async, List_ptr* captures) { HirExpr _r = {.tag=HirExpr_EClosure}; _r.data.EClosure.params = params; _r.data.EClosure.ret_ty = ret_ty; _r.data.EClosure.body = _tr_obj_retain(body); _r.data.EClosure.is_async = is_async; _r.data.EClosure.captures = captures; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ESuperMethodCall(TrStr base_class, TrStr method, List_ptr* args, AstType* ty) { HirExpr _r = {.tag=HirExpr_ESuperMethodCall}; _r.data.ESuperMethodCall.base_class = _tr_str_retain(base_class); _r.data.ESuperMethodCall.method = _tr_str_retain(method); _r.data.ESuperMethodCall.args = args; _r.data.ESuperMethodCall.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ESuperPropAccess(TrStr base_class, TrStr prop, AstType* ty) { HirExpr _r = {.tag=HirExpr_ESuperPropAccess}; _r.data.ESuperPropAccess.base_class = _tr_str_retain(base_class); _r.data.ESuperPropAccess.prop = _tr_str_retain(prop); _r.data.ESuperPropAccess.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EList(List_ptr* items, AstType* ty) { HirExpr _r = {.tag=HirExpr_EList}; _r.data.EList.items = items; _r.data.EList.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ESet(List_ptr* items, AstType* ty) { HirExpr _r = {.tag=HirExpr_ESet}; _r.data.ESet.items = items; _r.data.ESet.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EDict(List_ptr* keys, List_ptr* vals, AstType* ty) { HirExpr _r = {.tag=HirExpr_EDict}; _r.data.EDict.keys = keys; _r.data.EDict.vals = vals; _r.data.EDict.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ETuple(List_ptr* items, AstType* ty) { HirExpr _r = {.tag=HirExpr_ETuple}; _r.data.ETuple.items = items; _r.data.ETuple.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EListComp(HirExpr* element, List_ptr* generators, AstType* ty) { HirExpr _r = {.tag=HirExpr_EListComp}; _r.data.EListComp.element = element; _r.data.EListComp.generators = generators; _r.data.EListComp.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EGeneratorExpr(HirExpr* element, List_ptr* generators, AstType* ty) { HirExpr _r = {.tag=HirExpr_EGeneratorExpr}; _r.data.EGeneratorExpr.element = element; _r.data.EGeneratorExpr.generators = generators; _r.data.EGeneratorExpr.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ESlice(HirExpr* start, HirExpr* stop, HirExpr* step, AstType* ty) { HirExpr _r = {.tag=HirExpr_ESlice}; _r.data.ESlice.start = start; _r.data.ESlice.stop = stop; _r.data.ESlice.step = step; _r.data.ESlice.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EAwait(HirExpr* expr, AstType* ty) { HirExpr _r = {.tag=HirExpr_EAwait}; _r.data.EAwait.expr = expr; _r.data.EAwait.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EAwaitTimeout(HirExpr* expr, HirExpr* timeout_ms, AstType* ty) { HirExpr _r = {.tag=HirExpr_EAwaitTimeout}; _r.data.EAwaitTimeout.expr = expr; _r.data.EAwaitTimeout.timeout_ms = timeout_ms; _r.data.EAwaitTimeout.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EYield(HirExpr* expr, AstType* ty) { HirExpr _r = {.tag=HirExpr_EYield}; _r.data.EYield.expr = expr; _r.data.EYield.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ETry(HirBlock* try_body, List_ptr* catches, HirBlock* finally_b, AstType* ty) { HirExpr _r = {.tag=HirExpr_ETry}; _r.data.ETry.try_body = _tr_obj_retain(try_body); _r.data.ETry.catches = catches; _r.data.ETry.finally_b = _tr_obj_retain(finally_b); _r.data.ETry.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ERange(HirExpr* start, HirExpr* end, bool inclusive, AstType* ty) { HirExpr _r = {.tag=HirExpr_ERange}; _r.data.ERange.start = start; _r.data.ERange.end = end; _r.data.ERange.inclusive = inclusive; _r.data.ERange.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ESizeOf(AstType* target_ty, AstType* ty) { HirExpr _r = {.tag=HirExpr_ESizeOf}; _r.data.ESizeOf.target_ty = target_ty; _r.data.ESizeOf.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EIfElse(HirExpr* cond, HirExpr* then_e, HirExpr* else_e, AstType* ty) { HirExpr _r = {.tag=HirExpr_EIfElse}; _r.data.EIfElse.cond = cond; _r.data.EIfElse.then_e = then_e; _r.data.EIfElse.else_e = else_e; _r.data.EIfElse.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EDo(HirBlock* body, AstType* ty) { HirExpr _r = {.tag=HirExpr_EDo}; _r.data.EDo.body = _tr_obj_retain(body); _r.data.EDo.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EMatchExpr(HirExpr* subj, List_ptr* arms, AstType* ty) { HirExpr _r = {.tag=HirExpr_EMatchExpr}; _r.data.EMatchExpr.subj = subj; _r.data.EMatchExpr.arms = arms; _r.data.EMatchExpr.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_ELoop(HirBlock* body, AstType* ty) { HirExpr _r = {.tag=HirExpr_ELoop}; _r.data.ELoop.body = _tr_obj_retain(body); _r.data.ELoop.ty = ty; return _r; }
static inline __attribute__((always_inline)) HirExpr HirExpr_ctor_EWhileExpr(HirExpr* cond, HirBlock* body, HirBlock* else_body, AstType* ty) { HirExpr _r = {.tag=HirExpr_EWhileExpr}; _r.data.EWhileExpr.cond = cond; _r.data.EWhileExpr.body = _tr_obj_retain(body); _r.data.EWhileExpr.else_body = _tr_obj_retain(else_body); _r.data.EWhileExpr.ty = ty; return _r; }

typedef enum {
    HirStmt_SExpr,
    HirStmt_SLet,
    HirStmt_SAssign,
    HirStmt_SReturn,
    HirStmt_SBreak,
    HirStmt_SContinue,
    HirStmt_SPass,
    HirStmt_SRaise,
    HirStmt_SUnsafe,
    HirStmt_SIf,
    HirStmt_SWhile,
    HirStmt_SFor,
    HirStmt_SForUnpack,
    HirStmt_SMatch,
    HirStmt_STry,
    HirStmt_SAssert,
    HirStmt_SWith,
    HirStmt_SAsm,
    HirStmt_SSpawn,
    HirStmt_STaskGroup,
    HirStmt_SGpuBlock,
    HirStmt_SFree,
    HirStmt_SMultiLet,
    HirStmt_SChanSelect,
    HirStmt_SDefer,
    HirStmt_SAutoDrop,
    HirStmt_SLineMarker
} HirStmt_tag;

typedef struct HirStmt {
    HirStmt_tag tag;
    union {
        struct {
            HirExpr* expr;
        } SExpr;
        struct {
            TrStr name;
            Ownership ownership;
            bool is_mut;
            bool is_const;
            bool is_shared;
            AstType* ty;
            HirExpr* val;
        } SLet;
        struct {
            HirExpr* target;
            HirExpr* val;
        } SAssign;
        struct {
            HirExpr* val;
        } SReturn;
        struct {
            HirExpr* val;
        } SBreak;
        struct {
            HirExpr* val;
        } SRaise;
        struct {
            HirBlock* body;
        } SUnsafe;
        struct {
            HirExpr* cond;
            HirBlock* then_b;
            HirBlock* else_b;
        } SIf;
        struct {
            HirExpr* cond;
            HirBlock* body;
        } SWhile;
        struct {
            TrStr var;
            HirExpr* iter;
            HirBlock* body;
        } SFor;
        struct {
            List_TrStr* vars;
            HirExpr* iter;
            HirBlock* body;
        } SForUnpack;
        struct {
            HirExpr* expr;
            List_ptr* arms;
        } SMatch;
        struct {
            HirBlock* try_body;
            List_ptr* catches;
            HirBlock* finally_b;
        } STry;
        struct {
            HirExpr* cond;
            HirExpr* msg;
        } SAssert;
        struct {
            List_ptr* items;
            List_TrStr* aliases;
            HirBlock* body;
        } SWith;
        struct {
            TrStr code;
            TrStr outputs;
            TrStr inputs;
            TrStr clobbers;
        } SAsm;
        struct {
            HirExpr* expr;
        } SSpawn;
        struct {
            HirBlock* body;
        } STaskGroup;
        struct {
            HirBlock* body;
        } SGpuBlock;
        struct {
            TrStr name;
        } SFree;
        struct {
            List_TrStr* names;
            bool is_mut;
            HirExpr* val;
        } SMultiLet;
        struct {
            List_ptr* cases;
        } SChanSelect;
        struct {
            HirStmt* stmt;
        } SDefer;
        struct {
            TrStr name;
            TrStr class_name;
        } SAutoDrop;
        struct {
            long long n;
        } SLineMarker;
    } data;
} HirStmt;

static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SExpr(HirExpr* expr) { HirStmt _r = {.tag=HirStmt_SExpr}; _r.data.SExpr.expr = expr; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SLet(TrStr name, Ownership ownership, bool is_mut, bool is_const, bool is_shared, AstType* ty, HirExpr* val) { HirStmt _r = {.tag=HirStmt_SLet}; _r.data.SLet.name = _tr_str_retain(name); _r.data.SLet.ownership = ownership; _r.data.SLet.is_mut = is_mut; _r.data.SLet.is_const = is_const; _r.data.SLet.is_shared = is_shared; _r.data.SLet.ty = ty; _r.data.SLet.val = val; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SAssign(HirExpr* target, HirExpr* val) { HirStmt _r = {.tag=HirStmt_SAssign}; _r.data.SAssign.target = target; _r.data.SAssign.val = val; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SReturn(HirExpr* val) { HirStmt _r = {.tag=HirStmt_SReturn}; _r.data.SReturn.val = val; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SBreak(HirExpr* val) { HirStmt _r = {.tag=HirStmt_SBreak}; _r.data.SBreak.val = val; return _r; }
#define HirStmt_make_SContinue() ((HirStmt){.tag=HirStmt_SContinue})
#define HirStmt_make_SPass() ((HirStmt){.tag=HirStmt_SPass})
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SRaise(HirExpr* val) { HirStmt _r = {.tag=HirStmt_SRaise}; _r.data.SRaise.val = val; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SUnsafe(HirBlock* body) { HirStmt _r = {.tag=HirStmt_SUnsafe}; _r.data.SUnsafe.body = _tr_obj_retain(body); return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SIf(HirExpr* cond, HirBlock* then_b, HirBlock* else_b) { HirStmt _r = {.tag=HirStmt_SIf}; _r.data.SIf.cond = cond; _r.data.SIf.then_b = _tr_obj_retain(then_b); _r.data.SIf.else_b = _tr_obj_retain(else_b); return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SWhile(HirExpr* cond, HirBlock* body) { HirStmt _r = {.tag=HirStmt_SWhile}; _r.data.SWhile.cond = cond; _r.data.SWhile.body = _tr_obj_retain(body); return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SFor(TrStr var, HirExpr* iter, HirBlock* body) { HirStmt _r = {.tag=HirStmt_SFor}; _r.data.SFor.var = _tr_str_retain(var); _r.data.SFor.iter = iter; _r.data.SFor.body = _tr_obj_retain(body); return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SForUnpack(List_TrStr* vars, HirExpr* iter, HirBlock* body) { HirStmt _r = {.tag=HirStmt_SForUnpack}; _r.data.SForUnpack.vars = vars; _r.data.SForUnpack.iter = iter; _r.data.SForUnpack.body = _tr_obj_retain(body); return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SMatch(HirExpr* expr, List_ptr* arms) { HirStmt _r = {.tag=HirStmt_SMatch}; _r.data.SMatch.expr = expr; _r.data.SMatch.arms = arms; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_STry(HirBlock* try_body, List_ptr* catches, HirBlock* finally_b) { HirStmt _r = {.tag=HirStmt_STry}; _r.data.STry.try_body = _tr_obj_retain(try_body); _r.data.STry.catches = catches; _r.data.STry.finally_b = _tr_obj_retain(finally_b); return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SAssert(HirExpr* cond, HirExpr* msg) { HirStmt _r = {.tag=HirStmt_SAssert}; _r.data.SAssert.cond = cond; _r.data.SAssert.msg = msg; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SWith(List_ptr* items, List_TrStr* aliases, HirBlock* body) { HirStmt _r = {.tag=HirStmt_SWith}; _r.data.SWith.items = items; _r.data.SWith.aliases = aliases; _r.data.SWith.body = _tr_obj_retain(body); return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SAsm(TrStr code, TrStr outputs, TrStr inputs, TrStr clobbers) { HirStmt _r = {.tag=HirStmt_SAsm}; _r.data.SAsm.code = _tr_str_retain(code); _r.data.SAsm.outputs = _tr_str_retain(outputs); _r.data.SAsm.inputs = _tr_str_retain(inputs); _r.data.SAsm.clobbers = _tr_str_retain(clobbers); return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SSpawn(HirExpr* expr) { HirStmt _r = {.tag=HirStmt_SSpawn}; _r.data.SSpawn.expr = expr; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_STaskGroup(HirBlock* body) { HirStmt _r = {.tag=HirStmt_STaskGroup}; _r.data.STaskGroup.body = _tr_obj_retain(body); return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SGpuBlock(HirBlock* body) { HirStmt _r = {.tag=HirStmt_SGpuBlock}; _r.data.SGpuBlock.body = _tr_obj_retain(body); return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SFree(TrStr name) { HirStmt _r = {.tag=HirStmt_SFree}; _r.data.SFree.name = _tr_str_retain(name); return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SMultiLet(List_TrStr* names, bool is_mut, HirExpr* val) { HirStmt _r = {.tag=HirStmt_SMultiLet}; _r.data.SMultiLet.names = names; _r.data.SMultiLet.is_mut = is_mut; _r.data.SMultiLet.val = val; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SChanSelect(List_ptr* cases) { HirStmt _r = {.tag=HirStmt_SChanSelect}; _r.data.SChanSelect.cases = cases; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SDefer(HirStmt* stmt) { HirStmt _r = {.tag=HirStmt_SDefer}; _r.data.SDefer.stmt = stmt; return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SAutoDrop(TrStr name, TrStr class_name) { HirStmt _r = {.tag=HirStmt_SAutoDrop}; _r.data.SAutoDrop.name = _tr_str_retain(name); _r.data.SAutoDrop.class_name = _tr_str_retain(class_name); return _r; }
static inline __attribute__((always_inline)) HirStmt HirStmt_ctor_SLineMarker(long long n) { HirStmt _r = {.tag=HirStmt_SLineMarker}; _r.data.SLineMarker.n = n; return _r; }

typedef enum {
    MirStmt_MDeclare,
    MirStmt_MAssign,
    MirStmt_MEval
} MirStmt_tag;

typedef struct MirStmt {
    MirStmt_tag tag;
    union {
        struct {
            TrStr place;
            HirExpr* value;
        } MDeclare;
        struct {
            TrStr place;
            HirExpr* value;
        } MAssign;
        struct {
            HirExpr* value;
        } MEval;
    } data;
} MirStmt;

static inline __attribute__((always_inline)) MirStmt MirStmt_ctor_MDeclare(TrStr place, HirExpr* value) { MirStmt _r = {.tag=MirStmt_MDeclare}; _r.data.MDeclare.place = _tr_str_retain(place); _r.data.MDeclare.value = value; return _r; }
static inline __attribute__((always_inline)) MirStmt MirStmt_ctor_MAssign(TrStr place, HirExpr* value) { MirStmt _r = {.tag=MirStmt_MAssign}; _r.data.MAssign.place = _tr_str_retain(place); _r.data.MAssign.value = value; return _r; }
static inline __attribute__((always_inline)) MirStmt MirStmt_ctor_MEval(HirExpr* value) { MirStmt _r = {.tag=MirStmt_MEval}; _r.data.MEval.value = value; return _r; }

typedef enum {
    MirTerm_TGoto,
    MirTerm_TBranch,
    MirTerm_TReturn,
    MirTerm_TReturnVoid,
    MirTerm_TUnset
} MirTerm_tag;

typedef struct MirTerm {
    MirTerm_tag tag;
    union {
        struct {
            long long target;
        } TGoto;
        struct {
            HirExpr* cond;
            long long t;
            long long e;
        } TBranch;
        struct {
            HirExpr* value;
        } TReturn;
    } data;
} MirTerm;

static inline __attribute__((always_inline)) MirTerm MirTerm_ctor_TGoto(long long target) { MirTerm _r = {.tag=MirTerm_TGoto}; _r.data.TGoto.target = target; return _r; }
static inline __attribute__((always_inline)) MirTerm MirTerm_ctor_TBranch(HirExpr* cond, long long t, long long e) { MirTerm _r = {.tag=MirTerm_TBranch}; _r.data.TBranch.cond = cond; _r.data.TBranch.t = t; _r.data.TBranch.e = e; return _r; }
static inline __attribute__((always_inline)) MirTerm MirTerm_ctor_TReturn(HirExpr* value) { MirTerm _r = {.tag=MirTerm_TReturn}; _r.data.TReturn.value = value; return _r; }
#define MirTerm_make_TReturnVoid() ((MirTerm){.tag=MirTerm_TReturnVoid})
#define MirTerm_make_TUnset() ((MirTerm){.tag=MirTerm_TUnset})

typedef enum {
    SymbolKind_SFunction,
    SymbolKind_SClass,
    SymbolKind_SEnum,
    SymbolKind_SVariable,
    SymbolKind_SInterface
} SymbolKind_tag;

typedef struct SymbolKind {
    SymbolKind_tag tag;
} SymbolKind;

#define SymbolKind_make_SFunction() ((SymbolKind){.tag=SymbolKind_SFunction})
#define SymbolKind_make_SClass() ((SymbolKind){.tag=SymbolKind_SClass})
#define SymbolKind_make_SEnum() ((SymbolKind){.tag=SymbolKind_SEnum})
#define SymbolKind_make_SVariable() ((SymbolKind){.tag=SymbolKind_SVariable})
#define SymbolKind_make_SInterface() ((SymbolKind){.tag=SymbolKind_SInterface})

typedef enum {
    LType_LVoid,
    LType_LI64,
    LType_LPtr
} LType_tag;

typedef struct LType {
    LType_tag tag;
} LType;

#define LType_make_LVoid() ((LType){.tag=LType_LVoid})
#define LType_make_LI64() ((LType){.tag=LType_LI64})
#define LType_make_LPtr() ((LType){.tag=LType_LPtr})

typedef enum {
    LInst_IConst,
    LInst_IStr,
    LInst_IBinOp,
    LInst_ILoadVar,
    LInst_IStoreVar,
    LInst_ILoadGlobal,
    LInst_IStoreGlobal,
    LInst_ICall,
    LInst_IFBinOp,
    LInst_IIToF,
    LInst_IFToI,
    LInst_IFCall1,
    LInst_IFCallF,
    LInst_IFCall2F,
    LInst_IBitsF,
    LInst_IFBits,
    LInst_IAddrVar,
    LInst_IAddrGlobal,
    LInst_IFuncAddr,
    LInst_ICallInd,
    LInst_IAsm,
    LInst_ILoad,
    LInst_IStore,
    LInst_ILoadB,
    LInst_IStoreB
} LInst_tag;

typedef struct LInst {
    LInst_tag tag;
    union {
        struct {
            long long dst;
            long long v;
        } IConst;
        struct {
            long long dst;
            long long str_idx;
        } IStr;
        struct {
            long long dst;
            TrStr op;
            long long a;
            long long b;
        } IBinOp;
        struct {
            long long dst;
            TrStr name;
        } ILoadVar;
        struct {
            TrStr name;
            long long src;
        } IStoreVar;
        struct {
            long long dst;
            long long gidx;
        } ILoadGlobal;
        struct {
            long long gidx;
            long long src;
        } IStoreGlobal;
        struct {
            long long dst;
            TrStr callee;
            List_i64* args;
        } ICall;
        struct {
            long long dst;
            TrStr op;
            long long a;
            long long b;
        } IFBinOp;
        struct {
            long long dst;
            long long src;
        } IIToF;
        struct {
            long long dst;
            long long src;
        } IFToI;
        struct {
            long long dst;
            TrStr callee;
            long long arg;
        } IFCall1;
        struct {
            long long dst;
            TrStr callee;
            long long arg;
        } IFCallF;
        struct {
            long long dst;
            TrStr callee;
            long long a;
            long long b;
        } IFCall2F;
        struct {
            long long dst;
            long long src;
        } IBitsF;
        struct {
            long long dst;
            long long src;
        } IFBits;
        struct {
            long long dst;
            TrStr name;
        } IAddrVar;
        struct {
            long long dst;
            long long gidx;
        } IAddrGlobal;
        struct {
            long long dst;
            TrStr fname;
        } IFuncAddr;
        struct {
            long long dst;
            long long fnreg;
            List_i64* args;
        } ICallInd;
        struct {
            TrStr code;
            TrStr cons;
        } IAsm;
        struct {
            long long dst;
            long long base;
            long long off;
            long long aclass;
        } ILoad;
        struct {
            long long base;
            long long off;
            long long src;
            long long aclass;
        } IStore;
        struct {
            long long dst;
            long long base;
            long long off;
        } ILoadB;
        struct {
            long long base;
            long long off;
            long long src;
        } IStoreB;
    } data;
} LInst;

static inline __attribute__((always_inline)) LInst LInst_ctor_IConst(long long dst, long long v) { LInst _r = {.tag=LInst_IConst}; _r.data.IConst.dst = dst; _r.data.IConst.v = v; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IStr(long long dst, long long str_idx) { LInst _r = {.tag=LInst_IStr}; _r.data.IStr.dst = dst; _r.data.IStr.str_idx = str_idx; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IBinOp(long long dst, TrStr op, long long a, long long b) { LInst _r = {.tag=LInst_IBinOp}; _r.data.IBinOp.dst = dst; _r.data.IBinOp.op = _tr_str_retain(op); _r.data.IBinOp.a = a; _r.data.IBinOp.b = b; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_ILoadVar(long long dst, TrStr name) { LInst _r = {.tag=LInst_ILoadVar}; _r.data.ILoadVar.dst = dst; _r.data.ILoadVar.name = _tr_str_retain(name); return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IStoreVar(TrStr name, long long src) { LInst _r = {.tag=LInst_IStoreVar}; _r.data.IStoreVar.name = _tr_str_retain(name); _r.data.IStoreVar.src = src; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_ILoadGlobal(long long dst, long long gidx) { LInst _r = {.tag=LInst_ILoadGlobal}; _r.data.ILoadGlobal.dst = dst; _r.data.ILoadGlobal.gidx = gidx; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IStoreGlobal(long long gidx, long long src) { LInst _r = {.tag=LInst_IStoreGlobal}; _r.data.IStoreGlobal.gidx = gidx; _r.data.IStoreGlobal.src = src; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_ICall(long long dst, TrStr callee, List_i64* args) { LInst _r = {.tag=LInst_ICall}; _r.data.ICall.dst = dst; _r.data.ICall.callee = _tr_str_retain(callee); _r.data.ICall.args = args; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IFBinOp(long long dst, TrStr op, long long a, long long b) { LInst _r = {.tag=LInst_IFBinOp}; _r.data.IFBinOp.dst = dst; _r.data.IFBinOp.op = _tr_str_retain(op); _r.data.IFBinOp.a = a; _r.data.IFBinOp.b = b; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IIToF(long long dst, long long src) { LInst _r = {.tag=LInst_IIToF}; _r.data.IIToF.dst = dst; _r.data.IIToF.src = src; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IFToI(long long dst, long long src) { LInst _r = {.tag=LInst_IFToI}; _r.data.IFToI.dst = dst; _r.data.IFToI.src = src; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IFCall1(long long dst, TrStr callee, long long arg) { LInst _r = {.tag=LInst_IFCall1}; _r.data.IFCall1.dst = dst; _r.data.IFCall1.callee = _tr_str_retain(callee); _r.data.IFCall1.arg = arg; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IFCallF(long long dst, TrStr callee, long long arg) { LInst _r = {.tag=LInst_IFCallF}; _r.data.IFCallF.dst = dst; _r.data.IFCallF.callee = _tr_str_retain(callee); _r.data.IFCallF.arg = arg; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IFCall2F(long long dst, TrStr callee, long long a, long long b) { LInst _r = {.tag=LInst_IFCall2F}; _r.data.IFCall2F.dst = dst; _r.data.IFCall2F.callee = _tr_str_retain(callee); _r.data.IFCall2F.a = a; _r.data.IFCall2F.b = b; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IBitsF(long long dst, long long src) { LInst _r = {.tag=LInst_IBitsF}; _r.data.IBitsF.dst = dst; _r.data.IBitsF.src = src; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IFBits(long long dst, long long src) { LInst _r = {.tag=LInst_IFBits}; _r.data.IFBits.dst = dst; _r.data.IFBits.src = src; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IAddrVar(long long dst, TrStr name) { LInst _r = {.tag=LInst_IAddrVar}; _r.data.IAddrVar.dst = dst; _r.data.IAddrVar.name = _tr_str_retain(name); return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IAddrGlobal(long long dst, long long gidx) { LInst _r = {.tag=LInst_IAddrGlobal}; _r.data.IAddrGlobal.dst = dst; _r.data.IAddrGlobal.gidx = gidx; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IFuncAddr(long long dst, TrStr fname) { LInst _r = {.tag=LInst_IFuncAddr}; _r.data.IFuncAddr.dst = dst; _r.data.IFuncAddr.fname = _tr_str_retain(fname); return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_ICallInd(long long dst, long long fnreg, List_i64* args) { LInst _r = {.tag=LInst_ICallInd}; _r.data.ICallInd.dst = dst; _r.data.ICallInd.fnreg = fnreg; _r.data.ICallInd.args = args; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IAsm(TrStr code, TrStr cons) { LInst _r = {.tag=LInst_IAsm}; _r.data.IAsm.code = _tr_str_retain(code); _r.data.IAsm.cons = _tr_str_retain(cons); return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_ILoad(long long dst, long long base, long long off, long long aclass) { LInst _r = {.tag=LInst_ILoad}; _r.data.ILoad.dst = dst; _r.data.ILoad.base = base; _r.data.ILoad.off = off; _r.data.ILoad.aclass = aclass; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IStore(long long base, long long off, long long src, long long aclass) { LInst _r = {.tag=LInst_IStore}; _r.data.IStore.base = base; _r.data.IStore.off = off; _r.data.IStore.src = src; _r.data.IStore.aclass = aclass; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_ILoadB(long long dst, long long base, long long off) { LInst _r = {.tag=LInst_ILoadB}; _r.data.ILoadB.dst = dst; _r.data.ILoadB.base = base; _r.data.ILoadB.off = off; return _r; }
static inline __attribute__((always_inline)) LInst LInst_ctor_IStoreB(long long base, long long off, long long src) { LInst _r = {.tag=LInst_IStoreB}; _r.data.IStoreB.base = base; _r.data.IStoreB.off = off; _r.data.IStoreB.src = src; return _r; }

typedef enum {
    LTerm_TRetInt,
    LTerm_TRetVal,
    LTerm_TRetVoid,
    LTerm_TBr,
    LTerm_TCondBr,
    LTerm_TUnset
} LTerm_tag;

typedef struct LTerm {
    LTerm_tag tag;
    union {
        struct {
            long long v;
        } TRetInt;
        struct {
            long long v;
        } TRetVal;
        struct {
            long long target;
        } TBr;
        struct {
            long long cond;
            long long then_b;
            long long else_b;
        } TCondBr;
    } data;
} LTerm;

static inline __attribute__((always_inline)) LTerm LTerm_ctor_TRetInt(long long v) { LTerm _r = {.tag=LTerm_TRetInt}; _r.data.TRetInt.v = v; return _r; }
static inline __attribute__((always_inline)) LTerm LTerm_ctor_TRetVal(long long v) { LTerm _r = {.tag=LTerm_TRetVal}; _r.data.TRetVal.v = v; return _r; }
#define LTerm_make_TRetVoid() ((LTerm){.tag=LTerm_TRetVoid})
static inline __attribute__((always_inline)) LTerm LTerm_ctor_TBr(long long target) { LTerm _r = {.tag=LTerm_TBr}; _r.data.TBr.target = target; return _r; }
static inline __attribute__((always_inline)) LTerm LTerm_ctor_TCondBr(long long cond, long long then_b, long long else_b) { LTerm _r = {.tag=LTerm_TCondBr}; _r.data.TCondBr.cond = cond; _r.data.TCondBr.then_b = then_b; _r.data.TCondBr.else_b = else_b; return _r; }
#define LTerm_make_TUnset() ((LTerm){.tag=LTerm_TUnset})

typedef enum {
    MacroVal_MStr,
    MacroVal_MInt,
    MacroVal_MBool,
    MacroVal_MList,
    MacroVal_MRec,
    MacroVal_MNil
} MacroVal_tag;

typedef struct MacroVal {
    MacroVal_tag tag;
    union {
        struct {
            TrStr s;
        } MStr;
        struct {
            long long n;
        } MInt;
        struct {
            bool b;
        } MBool;
        struct {
            List_ptr* items;
        } MList;
        struct {
            List_TrStr* keys;
            List_ptr* vals;
        } MRec;
    } data;
} MacroVal;

static inline __attribute__((always_inline)) MacroVal MacroVal_ctor_MStr(TrStr s) { MacroVal _r = {.tag=MacroVal_MStr}; _r.data.MStr.s = _tr_str_retain(s); return _r; }
static inline __attribute__((always_inline)) MacroVal MacroVal_ctor_MInt(long long n) { MacroVal _r = {.tag=MacroVal_MInt}; _r.data.MInt.n = n; return _r; }
static inline __attribute__((always_inline)) MacroVal MacroVal_ctor_MBool(bool b) { MacroVal _r = {.tag=MacroVal_MBool}; _r.data.MBool.b = b; return _r; }
static inline __attribute__((always_inline)) MacroVal MacroVal_ctor_MList(List_ptr* items) { MacroVal _r = {.tag=MacroVal_MList}; _r.data.MList.items = items; return _r; }
static inline __attribute__((always_inline)) MacroVal MacroVal_ctor_MRec(List_TrStr* keys, List_ptr* vals) { MacroVal _r = {.tag=MacroVal_MRec}; _r.data.MRec.keys = keys; _r.data.MRec.vals = vals; return _r; }
#define MacroVal_make_MNil() ((MacroVal){.tag=MacroVal_MNil})

#ifndef StringObj_STRUCT_DEFINED
#define StringObj_STRUCT_DEFINED
typedef struct StringObj {
    size_t __rc;
    char* data;
    long long len;
    long long capacity;
} StringObj;
static void _trdrop_StringObj(void* vp) {
    StringObj* self = (StringObj*)vp; (void)self;
}
#endif

#ifndef StringBuilder_STRUCT_DEFINED
#define StringBuilder_STRUCT_DEFINED
typedef struct StringBuilder {
    size_t __rc;
    StringObj* buf;
} StringBuilder;
#endif

#ifndef Lexer_STRUCT_DEFINED
#define Lexer_STRUCT_DEFINED
typedef struct Lexer {
    size_t __rc;
    char* src;
    long long len;
    long long pos;
    long long line;
    long long line_start;
    long long tok_col;
    List_i64* indent_stack;
    long long pending_dedents;
    List_i64* token_lines;
    List_i64* token_cols;
    bool record_comments;
    List_i64* comment_lines;
    List_TrStr* comment_texts;
    List_bool* comment_trailing;
} Lexer;
static void _trdrop_Lexer(void* vp) {
    Lexer* self = (Lexer*)vp; (void)self;
    List_i64_free(self->indent_stack);
    List_i64_free(self->token_lines);
    List_i64_free(self->token_cols);
    List_i64_free(self->comment_lines);
    List_TrStr_free(self->comment_texts);
    List_bool_free(self->comment_trailing);
}
#endif

#ifndef AstType_STRUCT_DEFINED
#define AstType_STRUCT_DEFINED
typedef struct AstType {
    size_t __rc;
    TrStr name;
    List_ptr* args;
    TrStr from_param;
    List_TrStr* from_regions;
    long long from_index;
    bool is_borrow;
    bool is_mut_borrow;
    long long array_size;
} AstType;
static void _trdrop_AstType(void* vp) {
    AstType* self = (AstType*)vp; (void)self;
    _tr_str_release(self->name);
    _tr_str_release(self->from_param);
    List_TrStr_free(self->from_regions);
}
#endif

#ifndef GenericConstraint_STRUCT_DEFINED
#define GenericConstraint_STRUCT_DEFINED
typedef struct GenericConstraint {
    size_t __rc;
    TrStr target;
    List_ptr* bounds;
} GenericConstraint;
static void _trdrop_GenericConstraint(void* vp) {
    GenericConstraint* self = (GenericConstraint*)vp; (void)self;
    _tr_str_release(self->target);
    List_ptr_free(self->bounds);
}
#endif

#ifndef Decorator_STRUCT_DEFINED
#define Decorator_STRUCT_DEFINED
typedef struct Decorator {
    size_t __rc;
    TrStr name;
    List_ptr* args;
} Decorator;
static void _trdrop_Decorator(void* vp) {
    Decorator* self = (Decorator*)vp; (void)self;
    _tr_str_release(self->name);
}
#endif

#ifndef Comprehension_STRUCT_DEFINED
#define Comprehension_STRUCT_DEFINED
typedef struct Comprehension {
    size_t __rc;
    TrStr target;
    Expr* iter;
    List_ptr* ifs;
    bool is_async;
} Comprehension;
static void _trdrop_Comprehension(void* vp) {
    Comprehension* self = (Comprehension*)vp; (void)self;
    _tr_str_release(self->target);
}
#endif

#ifndef CatchClause_STRUCT_DEFINED
#define CatchClause_STRUCT_DEFINED
typedef struct CatchClause {
    size_t __rc;
    TrStr err_name;
    AstType** err_type;
    Block** body;
} CatchClause;
static void _trdrop_CatchClause(void* vp) {
    CatchClause* self = (CatchClause*)vp; (void)self;
    _tr_str_release(self->err_name);
}
#endif

#ifndef MatchArm_STRUCT_DEFINED
#define MatchArm_STRUCT_DEFINED
typedef struct MatchArm {
    size_t __rc;
    Pattern pat;
    Expr* guard;
    Block** body;
} MatchArm;
static void _trdrop_MatchArm(void* vp) {
    MatchArm* self = (MatchArm*)vp; (void)self;
}
#endif

#ifndef FStringPart_STRUCT_DEFINED
#define FStringPart_STRUCT_DEFINED
typedef struct FStringPart {
    size_t __rc;
    bool is_expr;
    TrStr text;
    Expr* expr;
    TrStr fmt_spec;
} FStringPart;
static void _trdrop_FStringPart(void* vp) {
    FStringPart* self = (FStringPart*)vp; (void)self;
    _tr_str_release(self->text);
    _tr_str_release(self->fmt_spec);
}
#endif

#ifndef ChanSelectArm_STRUCT_DEFINED
#define ChanSelectArm_STRUCT_DEFINED
typedef struct ChanSelectArm {
    size_t __rc;
    long long kind;
    Expr* chan_expr;
    Expr* val_expr;
    TrStr var_name;
    Expr* timeout_ms;
    Block* body;
} ChanSelectArm;
static void _trdrop_ChanSelectArm(void* vp) {
    ChanSelectArm* self = (ChanSelectArm*)vp; (void)self;
    _tr_str_release(self->var_name);
}
#endif

#ifndef Block_STRUCT_DEFINED
#define Block_STRUCT_DEFINED
typedef struct Block {
    size_t __rc;
    List_ptr* stmts;
} Block;
static void _trdrop_Block(void* vp) {
    Block* self = (Block*)vp; (void)self;
    List_ptr_free(self->stmts);
}
#endif

#ifndef ElifClause_STRUCT_DEFINED
#define ElifClause_STRUCT_DEFINED
typedef struct ElifClause {
    size_t __rc;
    Expr* cond;
    Block** body;
} ElifClause;
static void _trdrop_ElifClause(void* vp) {
    ElifClause* self = (ElifClause*)vp; (void)self;
}
#endif

#ifndef Param_STRUCT_DEFINED
#define Param_STRUCT_DEFINED
typedef struct Param {
    size_t __rc;
    TrStr name;
    AstType** ty;
    bool is_ref;
    bool is_mut_ref;
    bool is_variadic;
} Param;
static void _trdrop_Param(void* vp) {
    Param* self = (Param*)vp; (void)self;
    _tr_str_release(self->name);
}
#endif

#ifndef FunctionDef_STRUCT_DEFINED
#define FunctionDef_STRUCT_DEFINED
typedef struct FunctionDef {
    size_t __rc;
    TrStr name;
    List_TrStr* generics;
    List_ptr* params;
    AstType** ret_ty;
    AstType** throws_ty;
    List_ptr* decorators;
    List_ptr* constraints;
    bool is_variadic;
    bool is_async;
    bool is_extern;
    bool is_public;
    bool is_export;
    bool is_macro;
    bool is_lib;
    Block* body;
    long long line;
    List_TrStr* outlives_a;
    List_TrStr* outlives_b;
} FunctionDef;
static void _trdrop_FunctionDef(void* vp) {
    FunctionDef* self = (FunctionDef*)vp; (void)self;
    _tr_str_release(self->name);
    List_TrStr_free(self->outlives_a);
    List_TrStr_free(self->outlives_b);
}
#endif

#ifndef FieldDef_STRUCT_DEFINED
#define FieldDef_STRUCT_DEFINED
typedef struct FieldDef {
    size_t __rc;
    TrStr name;
    AstType** ty;
    Expr* default_val;
} FieldDef;
static void _trdrop_FieldDef(void* vp) {
    FieldDef* self = (FieldDef*)vp; (void)self;
    _tr_str_release(self->name);
}
#endif

#ifndef ClassDef_STRUCT_DEFINED
#define ClassDef_STRUCT_DEFINED
typedef struct ClassDef {
    size_t __rc;
    TrStr name;
    List_TrStr* generics;
    List_TrStr* base_classes;
    List_TrStr* iface_names;
    List_ptr* iface_targs;
    List_ptr* fields;
    List_ptr* methods;
    List_ptr* decorators;
    List_ptr* constraints;
    bool is_public;
    bool is_class;
    long long line;
    TrStr docstring;
    List_TrStr* region_params;
} ClassDef;
static void _trdrop_ClassDef(void* vp) {
    ClassDef* self = (ClassDef*)vp; (void)self;
    _tr_str_release(self->name);
    _tr_str_release(self->docstring);
    List_TrStr_free(self->region_params);
}
#endif

#ifndef VariantDef_STRUCT_DEFINED
#define VariantDef_STRUCT_DEFINED
typedef struct VariantDef {
    size_t __rc;
    TrStr name;
    List_ptr* fields;
} VariantDef;
static void _trdrop_VariantDef(void* vp) {
    VariantDef* self = (VariantDef*)vp; (void)self;
    _tr_str_release(self->name);
}
#endif

#ifndef EnumDef_STRUCT_DEFINED
#define EnumDef_STRUCT_DEFINED
typedef struct EnumDef {
    size_t __rc;
    TrStr name;
    List_TrStr* generics;
    List_TrStr* iface_names;
    List_ptr* variants;
    List_ptr* methods;
    List_ptr* decorators;
    bool is_public;
    long long line;
    List_TrStr* region_params;
} EnumDef;
static void _trdrop_EnumDef(void* vp) {
    EnumDef* self = (EnumDef*)vp; (void)self;
    _tr_str_release(self->name);
    List_TrStr_free(self->region_params);
}
#endif

#ifndef InterfaceDef_STRUCT_DEFINED
#define InterfaceDef_STRUCT_DEFINED
typedef struct InterfaceDef {
    size_t __rc;
    TrStr name;
    List_TrStr* generics;
    List_ptr* methods;
    bool is_public;
    long long line;
    List_ptr* decorators;
    List_TrStr* region_params;
} InterfaceDef;
static void _trdrop_InterfaceDef(void* vp) {
    InterfaceDef* self = (InterfaceDef*)vp; (void)self;
    _tr_str_release(self->name);
    List_TrStr_free(self->region_params);
}
#endif

#ifndef ImportItem_STRUCT_DEFINED
#define ImportItem_STRUCT_DEFINED
typedef struct ImportItem {
    size_t __rc;
    TrStr name;
    TrStr alias;
} ImportItem;
static void _trdrop_ImportItem(void* vp) {
    ImportItem* self = (ImportItem*)vp; (void)self;
    _tr_str_release(self->name);
    _tr_str_release(self->alias);
}
#endif

#ifndef Program_STRUCT_DEFINED
#define Program_STRUCT_DEFINED
typedef struct Program {
    size_t __rc;
    List_ptr* decls;
} Program;
static void _trdrop_Program(void* vp) {
    Program* self = (Program*)vp; (void)self;
}
#endif

#ifndef Parser_STRUCT_DEFINED
#define Parser_STRUCT_DEFINED
typedef struct Parser {
    size_t __rc;
    List_Token* tokens;
    long long pos;
    List_i64* lines;
    List_i64* cols;
    TrStr src_text;
    long long error_count;
    TrStr current_file;
    TrMap* import_aliases;
} Parser;
static void _trdrop_Parser(void* vp) {
    Parser* self = (Parser*)vp; (void)self;
    _tr_str_release(self->src_text);
    _tr_str_release(self->current_file);
    Dict_free_strval(self->import_aliases);
}
#endif

#ifndef ModuleResolver_STRUCT_DEFINED
#define ModuleResolver_STRUCT_DEFINED
typedef struct ModuleResolver {
    size_t __rc;
    TrMap* visited;
    List_ptr* all_decls;
    List_TrStr* search_paths;
    List_TrStr* mod_dot_paths;
    List_TrStr* mod_file_paths;
    List_TrStr* all_decl_modules;
    TrStr current_mod;
    long long parse_errors;
} ModuleResolver;
static void _trdrop_ModuleResolver(void* vp) {
    ModuleResolver* self = (ModuleResolver*)vp; (void)self;
    Dict_free(self->visited);
    List_ptr_free(self->all_decls);
    List_TrStr_free(self->search_paths);
    List_TrStr_free(self->mod_dot_paths);
    List_TrStr_free(self->mod_file_paths);
    List_TrStr_free(self->all_decl_modules);
    _tr_str_release(self->current_mod);
}
#endif

#ifndef HirComprehension_STRUCT_DEFINED
#define HirComprehension_STRUCT_DEFINED
typedef struct HirComprehension {
    size_t __rc;
    TrStr target;
    HirExpr* iter;
    List_ptr* ifs;
    bool is_async;
} HirComprehension;
static void _trdrop_HirComprehension(void* vp) {
    HirComprehension* self = (HirComprehension*)vp; (void)self;
    _tr_str_release(self->target);
}
#endif

#ifndef HirCatchClause_STRUCT_DEFINED
#define HirCatchClause_STRUCT_DEFINED
typedef struct HirCatchClause {
    size_t __rc;
    TrStr err_name;
    AstType* err_type;
    HirBlock* body;
} HirCatchClause;
static void _trdrop_HirCatchClause(void* vp) {
    HirCatchClause* self = (HirCatchClause*)vp; (void)self;
    _tr_str_release(self->err_name);
    _tr_obj_release(self->body, _trdrop_HirBlock);
}
#endif

#ifndef HirFStringPart_STRUCT_DEFINED
#define HirFStringPart_STRUCT_DEFINED
typedef struct HirFStringPart {
    size_t __rc;
    bool is_expr;
    TrStr text;
    HirExpr* expr;
    TrStr fmt_spec;
} HirFStringPart;
static void _trdrop_HirFStringPart(void* vp) {
    HirFStringPart* self = (HirFStringPart*)vp; (void)self;
    _tr_str_release(self->text);
    _tr_str_release(self->fmt_spec);
}
#endif

#ifndef HirMatchArm_STRUCT_DEFINED
#define HirMatchArm_STRUCT_DEFINED
typedef struct HirMatchArm {
    size_t __rc;
    Pattern pat;
    HirBlock* body;
    HirExpr* guard;
} HirMatchArm;
static void _trdrop_HirMatchArm(void* vp) {
    HirMatchArm* self = (HirMatchArm*)vp; (void)self;
    _tr_obj_release(self->body, _trdrop_HirBlock);
}
#endif

#ifndef HirChanSelectArm_STRUCT_DEFINED
#define HirChanSelectArm_STRUCT_DEFINED
typedef struct HirChanSelectArm {
    size_t __rc;
    long long kind;
    HirExpr* chan_expr;
    HirExpr* val_expr;
    TrStr var_name;
    HirExpr* timeout_ms;
    HirBlock* body;
} HirChanSelectArm;
static void _trdrop_HirChanSelectArm(void* vp) {
    HirChanSelectArm* self = (HirChanSelectArm*)vp; (void)self;
    _tr_str_release(self->var_name);
    _tr_obj_release(self->body, _trdrop_HirBlock);
}
#endif

#ifndef HirBlock_STRUCT_DEFINED
#define HirBlock_STRUCT_DEFINED
typedef struct HirBlock {
    size_t __rc;
    List_ptr* stmts;
} HirBlock;
static void _trdrop_HirBlock(void* vp) {
    HirBlock* self = (HirBlock*)vp; (void)self;
    List_ptr_free(self->stmts);
}
#endif

#ifndef HirParam_STRUCT_DEFINED
#define HirParam_STRUCT_DEFINED
typedef struct HirParam {
    size_t __rc;
    TrStr name;
    AstType* ty;
} HirParam;
static void _trdrop_HirParam(void* vp) {
    HirParam* self = (HirParam*)vp; (void)self;
    _tr_str_release(self->name);
}
#endif

#ifndef HirFunction_STRUCT_DEFINED
#define HirFunction_STRUCT_DEFINED
typedef struct HirFunction {
    size_t __rc;
    TrStr name;
    TrStr class_name;
    List_TrStr* generics;
    List_ptr* constraints;
    List_ptr* params;
    AstType* ret_ty;
    AstType* throws_ty;
    HirBlock* body;
    List_ptr* decorators;
    bool is_async;
    bool is_extern;
    bool is_public;
    bool is_export;
    bool is_static;
    bool is_variadic;
    bool is_decorator;
    List_TrStr* borrow_borrowers;
    List_TrStr* borrow_sources;
    List_TrStr* proven_borrows;
    bool returns_owned;
} HirFunction;
static void _trdrop_HirFunction(void* vp) {
    HirFunction* self = (HirFunction*)vp; (void)self;
    _tr_str_release(self->name);
    _tr_str_release(self->class_name);
    _tr_obj_release(self->body, _trdrop_HirBlock);
}
#endif

#ifndef HirField_STRUCT_DEFINED
#define HirField_STRUCT_DEFINED
typedef struct HirField {
    size_t __rc;
    TrStr name;
    AstType* ty;
    bool is_public;
} HirField;
static void _trdrop_HirField(void* vp) {
    HirField* self = (HirField*)vp; (void)self;
    _tr_str_release(self->name);
}
#endif

#ifndef HirClass_STRUCT_DEFINED
#define HirClass_STRUCT_DEFINED
typedef struct HirClass {
    size_t __rc;
    TrStr name;
    List_TrStr* generics;
    List_TrStr* base_classes;
    List_TrStr* iface_names;
    List_ptr* iface_targs;
    List_ptr* fields;
    List_ptr* methods;
    List_ptr* decorators;
    bool is_public;
    bool is_class;
} HirClass;
static void _trdrop_HirClass(void* vp) {
    HirClass* self = (HirClass*)vp; (void)self;
    _tr_str_release(self->name);
}
#endif

#ifndef HirVariant_STRUCT_DEFINED
#define HirVariant_STRUCT_DEFINED
typedef struct HirVariant {
    size_t __rc;
    TrStr name;
    List_ptr* fields;
} HirVariant;
static void _trdrop_HirVariant(void* vp) {
    HirVariant* self = (HirVariant*)vp; (void)self;
    _tr_str_release(self->name);
}
#endif

#ifndef HirEnum_STRUCT_DEFINED
#define HirEnum_STRUCT_DEFINED
typedef struct HirEnum {
    size_t __rc;
    TrStr name;
    List_TrStr* generics;
    List_TrStr* iface_names;
    List_ptr* variants;
    List_ptr* methods;
    List_ptr* decorators;
    bool is_public;
} HirEnum;
static void _trdrop_HirEnum(void* vp) {
    HirEnum* self = (HirEnum*)vp; (void)self;
    _tr_str_release(self->name);
}
#endif

#ifndef HirInterface_STRUCT_DEFINED
#define HirInterface_STRUCT_DEFINED
typedef struct HirInterface {
    size_t __rc;
    TrStr name;
    List_TrStr* generics;
    List_ptr* methods;
    bool is_public;
} HirInterface;
static void _trdrop_HirInterface(void* vp) {
    HirInterface* self = (HirInterface*)vp; (void)self;
    _tr_str_release(self->name);
}
#endif

#ifndef HirProgram_STRUCT_DEFINED
#define HirProgram_STRUCT_DEFINED
typedef struct HirProgram {
    size_t __rc;
    List_ptr* functions;
    List_ptr* classes;
    List_ptr* enums;
    List_ptr* interfaces;
    List_ptr* top_level_stmts;
    List_ptr* extern_funcs;
    List_ptr* decorator_defs;
    List_TrStr* type_alias_names;
    List_ptr* type_alias_types;
} HirProgram;
static void _trdrop_HirProgram(void* vp) {
    HirProgram* self = (HirProgram*)vp; (void)self;
    List_ptr_free_obj(self->functions, _trdrop_HirFunction);
    List_ptr_free_obj(self->classes, _trdrop_HirClass);
    List_ptr_free_obj(self->enums, _trdrop_HirEnum);
    List_ptr_free_obj(self->interfaces, _trdrop_HirInterface);
    List_ptr_free(self->top_level_stmts);
    List_ptr_free_obj(self->extern_funcs, _trdrop_HirFunction);
    List_ptr_free_obj(self->decorator_defs, _trdrop_HirFunction);
    List_TrStr_free(self->type_alias_names);
    List_ptr_free(self->type_alias_types);
}
#endif

#ifndef MirBlock_STRUCT_DEFINED
#define MirBlock_STRUCT_DEFINED
typedef struct MirBlock {
    size_t __rc;
    long long id;
    List_ptr* stmts;
    MirTerm* term;
    HirBlock* hir_block;
} MirBlock;
static void _trdrop_MirBlock(void* vp) {
    MirBlock* self = (MirBlock*)vp; (void)self;
    List_ptr_free(self->stmts);
    _tr_obj_release(self->hir_block, _trdrop_HirBlock);
}
#endif

#ifndef DropSite_STRUCT_DEFINED
#define DropSite_STRUCT_DEFINED
typedef struct DropSite {
    size_t __rc;
    HirBlock* hir_block;
    List_TrStr* places;
} DropSite;
static void _trdrop_DropSite(void* vp) {
    DropSite* self = (DropSite*)vp; (void)self;
    _tr_obj_release(self->hir_block, _trdrop_HirBlock);
    List_TrStr_free(self->places);
}
#endif

#ifndef BorrowEdge_STRUCT_DEFINED
#define BorrowEdge_STRUCT_DEFINED
typedef struct BorrowEdge {
    size_t __rc;
    TrStr borrower;
    TrStr source;
    long long decl_block;
    bool is_exclusive;
    bool via_collection;
} BorrowEdge;
static void _trdrop_BorrowEdge(void* vp) {
    BorrowEdge* self = (BorrowEdge*)vp; (void)self;
    _tr_str_release(self->borrower);
    _tr_str_release(self->source);
}
#endif

#ifndef MirFunction_STRUCT_DEFINED
#define MirFunction_STRUCT_DEFINED
typedef struct MirFunction {
    size_t __rc;
    TrStr name;
    List_TrStr* params;
    List_ptr* blocks;
    List_ptr* if_bodies;
    LiveSet* unsafe_pinned;
    List_ptr* borrows;
    bool complete;
} MirFunction;
static void _trdrop_MirFunction(void* vp) {
    MirFunction* self = (MirFunction*)vp; (void)self;
    _tr_str_release(self->name);
    _tr_obj_release(self->unsafe_pinned, _trdrop_LiveSet);
}
#endif

#ifndef MirProgram_STRUCT_DEFINED
#define MirProgram_STRUCT_DEFINED
typedef struct MirProgram {
    size_t __rc;
    List_ptr* functions;
} MirProgram;
static void _trdrop_MirProgram(void* vp) {
    MirProgram* self = (MirProgram*)vp; (void)self;
    List_ptr_free_obj(self->functions, _trdrop_MirFunction);
}
#endif

#ifndef MirBuilder_STRUCT_DEFINED
#define MirBuilder_STRUCT_DEFINED
typedef struct MirBuilder {
    size_t __rc;
    List_ptr* blocks;
    long long cur;
    bool complete;
    HirBlock* cur_hb;
    List_ptr* if_bodies;
    long long in_unsafe;
    LiveSet* unsafe_pinned;
    List_i64* loop_continue;
    List_i64* loop_break;
    List_ptr* borrows;
} MirBuilder;
static void _trdrop_MirBuilder(void* vp) {
    MirBuilder* self = (MirBuilder*)vp; (void)self;
    _tr_obj_release(self->cur_hb, _trdrop_HirBlock);
    _tr_obj_release(self->unsafe_pinned, _trdrop_LiveSet);
    List_i64_free(self->loop_continue);
    List_i64_free(self->loop_break);
}
#endif

#ifndef LiveSet_STRUCT_DEFINED
#define LiveSet_STRUCT_DEFINED
typedef struct LiveSet {
    size_t __rc;
    List_TrStr* items;
} LiveSet;
static void _trdrop_LiveSet(void* vp) {
    LiveSet* self = (LiveSet*)vp; (void)self;
}
#endif

#ifndef Symbol_STRUCT_DEFINED
#define Symbol_STRUCT_DEFINED
typedef struct Symbol {
    size_t __rc;
    TrStr name;
    SymbolKind kind;
    AstType** ty;
    long long scope_depth;
    bool is_mut;
    bool is_const;
    bool is_shared;
    bool is_moved;
    long long active_borrows;
    List_TrStr* borrowed_by;
    bool is_init;
    bool is_param;
    bool is_maybe_moved;
    bool is_maybe_init;
    long long ptr_region;
    bool is_freed;
    long long decl_block_depth;
    long long decl_block_id;
    bool str_escaped;
    bool coll_escaped;
    TrStr borrows_region;
} Symbol;
static void _trdrop_Symbol(void* vp) {
    Symbol* self = (Symbol*)vp; (void)self;
    _tr_str_release(self->name);
    List_TrStr_free(self->borrowed_by);
    _tr_str_release(self->borrows_region);
}
#endif

#ifndef Scope_STRUCT_DEFINED
#define Scope_STRUCT_DEFINED
typedef struct Scope {
    size_t __rc;
    TrMap* variables;
    List_TrStr* decl_order;
} Scope;
static void _trdrop_Scope(void* vp) {
    Scope* self = (Scope*)vp; (void)self;
    Dict_free(self->variables);
    List_TrStr_free(self->decl_order);
}
#endif

#ifndef Sema_STRUCT_DEFINED
#define Sema_STRUCT_DEFINED
typedef struct Sema {
    size_t __rc;
    TrMap* globals;
    List_ptr* scopes;
    List_TrStr* errors;
    List_TrStr* warnings;
    TrMap* classes;
    TrMap* enums;
    TrMap* interfaces;
    TrMap* type_aliases;
    TrMap* type_alias_elem;
    TrStr current_file;
    TrStr current_func_name;
    TrStr current_class_name;
    long long current_scope_depth;
    bool in_async_fn;
    TrMap* assign_froms;
    TrMap* fn_sigs;
    TrMap* extern_names;
    List_ptr* nested_classes;
    List_ptr* nested_functions;
    List_ptr* nested_enums;
    List_ptr* nested_interfaces;
    long long current_line;
    List_TrStr* current_func_generics;
    List_ptr* current_func_constraints;
    long long closure_boundary;
    List_ptr* closure_caps;
    TrMap* closure_cap_set;
    bool in_assign_target;
    bool in_recv_pos;
    TrMap* container_borrows;
    bool capturing_moves;
    List_TrStr* branch_moved_buf;
    bool capturing_inits;
    List_TrStr* branch_init_buf;
    TrMap* copy_classes;
    bool in_unsafe;
    bool cur_fn_is_lib;
    TrStr current_func_ret_from;
    bool current_func_ret_borrow_str;
    List_TrStr* current_func_ret_regions;
    List_TrStr* current_func_outlives_a;
    List_TrStr* current_func_outlives_b;
    List_TrStr* current_region_params;
    List_TrStr* cur_func_borrowers;
    List_TrStr* cur_func_sources;
    bool strict_mode;
    bool no_heap;
    bool heap_boxes_tuples;
    TrMap* mutating_methods;
    TrMap* fn_ret_owned;
    TrMap* fn_param_consumes;
    TrMap* ptr_aliased;
    TrMap* decorator_names;
    TrMap* variadic_fns;
    TrMap* variadic_elem_ty;
    TrMap* fn_defs;
    List_i64* loop_scope_base;
    List_i64* fn_scope_base;
    long long block_depth;
    List_i64* block_depth_stack;
    long long next_block_id;
    List_i64* block_stack;
    List_i64* block_stack_base;
    long long do_temp_ctr;
} Sema;
static void _trdrop_Sema(void* vp) {
    Sema* self = (Sema*)vp; (void)self;
    Dict_free(self->globals);
    List_ptr_free_obj(self->scopes, _trdrop_Scope);
    List_TrStr_free(self->warnings);
    Dict_free_objval(self->classes, _trdrop_ClassDef);
    Dict_free_objval(self->enums, _trdrop_EnumDef);
    Dict_free_objval(self->interfaces, _trdrop_InterfaceDef);
    Dict_free_strval(self->type_aliases);
    Dict_free_strval(self->type_alias_elem);
    _tr_str_release(self->current_file);
    _tr_str_release(self->current_func_name);
    _tr_str_release(self->current_class_name);
    Dict_free(self->assign_froms);
    Dict_free(self->fn_sigs);
    Dict_free(self->extern_names);
    List_ptr_free_obj(self->nested_classes, _trdrop_HirClass);
    List_ptr_free_obj(self->nested_functions, _trdrop_HirFunction);
    List_ptr_free_obj(self->nested_enums, _trdrop_HirEnum);
    List_ptr_free_obj(self->nested_interfaces, _trdrop_HirInterface);
    List_ptr_free_obj(self->closure_caps, _trdrop_HirParam);
    Dict_free_strval(self->container_borrows);
    Dict_free(self->copy_classes);
    _tr_str_release(self->current_func_ret_from);
    List_TrStr_free(self->cur_func_borrowers);
    List_TrStr_free(self->cur_func_sources);
    Dict_free(self->mutating_methods);
    Dict_free(self->fn_ret_owned);
    Dict_free(self->fn_param_consumes);
    Dict_free(self->ptr_aliased);
    Dict_free(self->decorator_names);
    Dict_free_strval(self->variadic_fns);
    Dict_free(self->variadic_elem_ty);
    Dict_free_objval(self->fn_defs, _trdrop_FunctionDef);
    List_i64_free(self->loop_scope_base);
    List_i64_free(self->fn_scope_base);
    List_i64_free(self->block_depth_stack);
    List_i64_free(self->block_stack);
    List_i64_free(self->block_stack_base);
}
#endif

#ifndef Formatter_STRUCT_DEFINED
#define Formatter_STRUCT_DEFINED
typedef struct Formatter {
    size_t __rc;
    StringBuilder* out;
    long long indent;
    List_i64* c_lines;
    List_TrStr* c_texts;
    List_bool* c_trailing;
    long long ci;
    bool unsupported;
    bool in_fstring;
} Formatter;
static void _trdrop_Formatter(void* vp) {
    Formatter* self = (Formatter*)vp; (void)self;
}
#endif

#ifndef CTok_STRUCT_DEFINED
#define CTok_STRUCT_DEFINED
typedef struct CTok {
    size_t __rc;
    long long kind;
    TrStr text;
} CTok;
static void _trdrop_CTok(void* vp) {
    CTok* self = (CTok*)vp; (void)self;
    _tr_str_release(self->text);
}
#endif

#ifndef Bindgen_STRUCT_DEFINED
#define Bindgen_STRUCT_DEFINED
typedef struct Bindgen {
    size_t __rc;
    List_ptr* toks;
    long long pos;
    StringBuilder* funcs;
    StringBuilder* structs;
    StringBuilder* types;
    StringBuilder* consts;
    long long n_funcs;
    long long n_structs;
    TrMap* seen;
    List_TrStr* defined;
    TrMap* skip_syms;
    long long n_skipped;
} Bindgen;
static void _trdrop_Bindgen(void* vp) {
    Bindgen* self = (Bindgen*)vp; (void)self;
    Dict_free(self->seen);
    List_TrStr_free(self->defined);
}
#endif

#ifndef CppType_STRUCT_DEFINED
#define CppType_STRUCT_DEFINED
typedef struct CppType {
    size_t __rc;
    TrStr base;
    long long ptr;
    bool was_ptr;
    bool was_ref;
    bool is_prim;
} CppType;
static void _trdrop_CppType(void* vp) {
    CppType* self = (CppType*)vp; (void)self;
    _tr_str_release(self->base);
}
#endif

#ifndef CGenerator_STRUCT_DEFINED
#define CGenerator_STRUCT_DEFINED
typedef struct CGenerator {
    size_t __rc;
    StringBuilder* buf;
    StringBuilder* fwd_buf;
    StringBuilder* struct_buf;
    StringBuilder* list_types_buf;
    StringBuilder* proto_buf;
    StringBuilder* mono_buf;
    TrMap* mtxcd_emitted;
    List_TrStr* mtx_cd_specs;
    StringBuilder* fn_mono_body;
    long long temp_count;
    TrMap* classes;
    TrMap* enums;
    TrMap* interfaces;
    TrMap* functions;
    TrMap* method_owners;
    TrMap* decl_vars;
    TrMap* str_local_names;
    TrMap* class_local_names;
    TrMap* class_local_mono;
    TrMap* fn_owned;
    TrMap* raw_aliased;
    TrMap* coll_field_owned;
    TrMap* coll_field_disq;
    TrMap* cur_proven_borrows;
    bool cur_ret_is_borrow;
    AstType* cur_ret_ty;
    bool eliding_get_retain;
    bool no_elide;
    TrStr tier_define;
    TrStr bare_arch;
    bool cur_self_is_ptr;
    TrMap* coll_local_sfx;
    TrMap* coll_local_idict;
    TrMap* coll_local_strval;
    TrMap* coll_local_vtcoll;
    TrMap* type_subst;
    TrMap* mono_done;
    TrStr emit_fn_name;
    TrMap* list_type_done;
    TrMap* list_fwd_done;
    TrMap* elem_fmt_done;
    TrStr cur_class;
    TrStr cur_func;
    long long closure_count;
    TrMap* emitted_fns;
    TrMap* spawn_wrappers;
    TrMap* async_wrappers;
    TrMap* prescanned_fns;
    TrMap* shared_vars;
    TrStr cur_throws_ty;
    long long in_task_group;
    long long in_gpu_block;
    TrMap* value_types;
    List_TrStr* value_list_elems;
    List_TrStr* value_dict_elems;
    List_TrStr* value_set_elems;
    TrMap* global_vars;
    TrMap* closure_cap_set;
    TrStr closure_env_var;
    StringBuilder* closure_buf;
    TrStr last_clo_cname;
    TrStr last_clo_estruct;
    TrStr last_clo_ptypes;
    TrStr last_clo_ret;
    TrStr last_clo_init;
    TrMap* decorator_defs;
    TrMap* overloaded_sigs;
    TrMap* type_alias_map;
    TrMap* type_alias_tr;
    List_TrStr* defer_stack;
    List_TrStr* wrap_temp_decls;
    List_TrStr* wrap_temp_names;
    List_TrStr* wrap_obj_names;
    List_TrStr* wrap_obj_drops;
    List_TrStr* loop_res_stack;
    List_TrStr* loop_done_stack;
    bool emit_line_info;
    TrStr cur_src_file;
} CGenerator;
static void _trdrop_CGenerator(void* vp) {
    CGenerator* self = (CGenerator*)vp; (void)self;
    Dict_free(self->mtxcd_emitted);
    List_TrStr_free(self->mtx_cd_specs);
    Dict_free_objval(self->classes, _trdrop_HirClass);
    Dict_free_objval(self->enums, _trdrop_HirEnum);
    Dict_free_objval(self->interfaces, _trdrop_HirInterface);
    Dict_free_objval(self->functions, _trdrop_HirFunction);
    Dict_free(self->method_owners);
    Dict_free(self->decl_vars);
    Dict_free(self->str_local_names);
    Dict_free(self->class_local_names);
    Dict_free_strval(self->class_local_mono);
    Dict_free(self->fn_owned);
    Dict_free(self->raw_aliased);
    Dict_free(self->coll_field_owned);
    Dict_free(self->coll_field_disq);
    Dict_free(self->cur_proven_borrows);
    _tr_str_release(self->tier_define);
    _tr_str_release(self->bare_arch);
    Dict_free_strval(self->coll_local_sfx);
    Dict_free(self->coll_local_idict);
    Dict_free(self->coll_local_strval);
    Dict_free_strval(self->coll_local_vtcoll);
    Dict_free(self->mono_done);
    _tr_str_release(self->emit_fn_name);
    Dict_free(self->list_type_done);
    Dict_free(self->list_fwd_done);
    Dict_free(self->elem_fmt_done);
    _tr_str_release(self->cur_class);
    _tr_str_release(self->cur_func);
    Dict_free(self->emitted_fns);
    Dict_free(self->spawn_wrappers);
    Dict_free(self->async_wrappers);
    Dict_free(self->prescanned_fns);
    Dict_free_strval(self->shared_vars);
    _tr_str_release(self->cur_throws_ty);
    Dict_free(self->value_types);
    List_TrStr_free(self->value_list_elems);
    List_TrStr_free(self->value_dict_elems);
    List_TrStr_free(self->value_set_elems);
    Dict_free(self->global_vars);
    _tr_str_release(self->closure_env_var);
    _tr_str_release(self->last_clo_cname);
    _tr_str_release(self->last_clo_estruct);
    _tr_str_release(self->last_clo_ptypes);
    _tr_str_release(self->last_clo_ret);
    _tr_str_release(self->last_clo_init);
    Dict_free_objval(self->decorator_defs, _trdrop_HirFunction);
    Dict_free(self->overloaded_sigs);
    Dict_free_strval(self->type_alias_map);
    Dict_free_strval(self->type_alias_tr);
    List_TrStr_free(self->defer_stack);
    List_TrStr_free(self->wrap_temp_decls);
    List_TrStr_free(self->wrap_temp_names);
    List_TrStr_free(self->wrap_obj_names);
    List_TrStr_free(self->wrap_obj_drops);
    List_TrStr_free(self->loop_res_stack);
    List_TrStr_free(self->loop_done_stack);
    _tr_str_release(self->cur_src_file);
}
#endif

#ifndef CppExporter_STRUCT_DEFINED
#define CppExporter_STRUCT_DEFINED
typedef struct CppExporter {
    size_t __rc;
    CGenerator* gen;
    TrMap* cls_set;
    TrMap* list_elems;
    TrMap* dict_variants;
    bool needs_tuple;
    bool needs_list_ptr;
} CppExporter;
static void _trdrop_CppExporter(void* vp) {
    CppExporter* self = (CppExporter*)vp; (void)self;
    _tr_obj_release(self->gen, _trdrop_CGenerator);
    Dict_free(self->cls_set);
    Dict_free_strval(self->list_elems);
    Dict_free_strval(self->dict_variants);
}
#endif

#ifndef LBlock_STRUCT_DEFINED
#define LBlock_STRUCT_DEFINED
typedef struct LBlock {
    size_t __rc;
    long long id;
    List_ptr* insts;
    LTerm term;
} LBlock;
static void _trdrop_LBlock(void* vp) {
    LBlock* self = (LBlock*)vp; (void)self;
    List_ptr_free(self->insts);
}
#endif

#ifndef LFunc_STRUCT_DEFINED
#define LFunc_STRUCT_DEFINED
typedef struct LFunc {
    size_t __rc;
    TrStr name;
    bool is_main;
    List_ptr* blocks;
    long long cur;
    long long n_vregs;
    List_i64* vreg_types;
    List_TrStr* vars;
    List_i64* var_types;
    List_TrStr* var_cls;
    List_i64* var_arr;
    List_TrStr* params;
    long long tmp_ctr;
    List_i64* loop_cont;
    List_i64* loop_brk;
    List_i64* fresh_strs;
    List_i64* fresh_objs;
    List_TrStr* captures;
    List_i64* cap_tags;
    List_i64* var_xret;
    List_i64* vreg_xret;
    List_i64* try_blks;
    List_TrStr* try_msgs;
    List_ptr* defers;
    List_TrStr* mutex_unlocks;
    long long blk_depth;
    bool in_defer;
    bool is_throws;
    long long throws_ok_tag;
    long long throws_err_tag;
    long long in_taskgroup;
} LFunc;
static void _trdrop_LFunc(void* vp) {
    LFunc* self = (LFunc*)vp; (void)self;
    _tr_str_release(self->name);
    List_i64_free(self->vreg_types);
    List_TrStr_free(self->vars);
    List_i64_free(self->var_types);
    List_TrStr_free(self->var_cls);
    List_i64_free(self->var_arr);
    List_i64_free(self->loop_cont);
    List_i64_free(self->loop_brk);
    List_i64_free(self->fresh_strs);
    List_i64_free(self->fresh_objs);
    List_TrStr_free(self->captures);
    List_i64_free(self->cap_tags);
    List_i64_free(self->var_xret);
    List_i64_free(self->vreg_xret);
    List_i64_free(self->try_blks);
    List_TrStr_free(self->try_msgs);
    List_ptr_free(self->defers);
    List_TrStr_free(self->mutex_unlocks);
}
#endif

#ifndef ClassLayout_STRUCT_DEFINED
#define ClassLayout_STRUCT_DEFINED
typedef struct ClassLayout {
    size_t __rc;
    TrStr name;
    TrStr base;
    List_TrStr* fields;
    List_i64* ftags;
    List_TrStr* fcls;
    bool is_value;
} ClassLayout;
static void _trdrop_ClassLayout(void* vp) {
    ClassLayout* self = (ClassLayout*)vp; (void)self;
    _tr_str_release(self->name);
    _tr_str_release(self->base);
    List_i64_free(self->ftags);
    List_TrStr_free(self->fcls);
}
#endif

#ifndef VariantLayout_STRUCT_DEFINED
#define VariantLayout_STRUCT_DEFINED
typedef struct VariantLayout {
    size_t __rc;
    TrStr name;
    List_TrStr* fields;
    List_i64* ftags;
    List_TrStr* fcls;
} VariantLayout;
static void _trdrop_VariantLayout(void* vp) {
    VariantLayout* self = (VariantLayout*)vp; (void)self;
    _tr_str_release(self->name);
    List_i64_free(self->ftags);
    List_TrStr_free(self->fcls);
}
#endif

#ifndef EnumLayout_STRUCT_DEFINED
#define EnumLayout_STRUCT_DEFINED
typedef struct EnumLayout {
    size_t __rc;
    TrStr name;
    List_ptr* variants;
} EnumLayout;
static void _trdrop_EnumLayout(void* vp) {
    EnumLayout* self = (EnumLayout*)vp; (void)self;
    _tr_str_release(self->name);
}
#endif

#ifndef LModule_STRUCT_DEFINED
#define LModule_STRUCT_DEFINED
typedef struct LModule {
    size_t __rc;
    List_ptr* funcs;
    List_ptr* classes;
    List_ptr* enums;
    List_TrStr* ifaces;
    List_TrStr* externs;
    List_TrStr* fn_names;
    List_i64* fn_ret;
    List_TrStr* strings;
    List_TrStr* globals;
    List_i64* global_types;
    List_i64* global_arr;
    List_ptr* global_inits;
    HirProgram* hir_prog;
    bool ok;
    TrStr fail_note;
    List_TrStr* unavail_names;
    List_TrStr* unavail_notes;
    List_TrStr* subst_names;
    List_ptr* subst_tys;
    List_TrStr* extfn_names;
    List_i64* extfn_ret;
    List_TrStr* fn_owned_names;
    List_TrStr* overloaded_sigs;
    bool target_llvm;
    bool no_heap;
} LModule;
static void _trdrop_LModule(void* vp) {
    LModule* self = (LModule*)vp; (void)self;
    List_ptr_free_obj(self->classes, _trdrop_ClassLayout);
    List_ptr_free_obj(self->enums, _trdrop_EnumLayout);
    List_TrStr_free(self->ifaces);
    List_TrStr_free(self->externs);
    List_TrStr_free(self->fn_names);
    List_i64_free(self->fn_ret);
    List_TrStr_free(self->strings);
    List_TrStr_free(self->globals);
    List_i64_free(self->global_types);
    List_i64_free(self->global_arr);
    List_ptr_free(self->global_inits);
    _tr_obj_release(self->hir_prog, _trdrop_HirProgram);
    _tr_str_release(self->fail_note);
    List_TrStr_free(self->unavail_names);
    List_TrStr_free(self->unavail_notes);
    List_TrStr_free(self->subst_names);
    List_ptr_free(self->subst_tys);
    List_TrStr_free(self->extfn_names);
    List_i64_free(self->extfn_ret);
    List_TrStr_free(self->fn_owned_names);
    List_TrStr_free(self->overloaded_sigs);
}
#endif

#ifndef LlvmEmitter_STRUCT_DEFINED
#define LlvmEmitter_STRUCT_DEFINED
typedef struct LlvmEmitter {
    size_t __rc;
    StringBuilder* out;
    LModule* m;
    long long tmp;
    LFunc* cur;
    TrStr cur_ret;
    bool cur_main;
} LlvmEmitter;
static void _trdrop_LlvmEmitter(void* vp) {
    LlvmEmitter* self = (LlvmEmitter*)vp; (void)self;
    _tr_obj_release(self->m, _trdrop_LModule);
    _tr_obj_release(self->cur, _trdrop_LFunc);
    _tr_str_release(self->cur_ret);
}
#endif

#ifndef LlvmGenerator_STRUCT_DEFINED
#define LlvmGenerator_STRUCT_DEFINED
typedef struct LlvmGenerator {
    size_t __rc;
    bool ok;
    TrStr fail_note;
} LlvmGenerator;
static void _trdrop_LlvmGenerator(void* vp) {
    LlvmGenerator* self = (LlvmGenerator*)vp; (void)self;
    _tr_str_release(self->fail_note);
}
#endif

#ifndef ByteBuf_STRUCT_DEFINED
#define ByteBuf_STRUCT_DEFINED
typedef struct ByteBuf {
    size_t __rc;
    unsigned char* data;
    long long len;
    long long cap;
} ByteBuf;
static void _trdrop_ByteBuf(void* vp) {
    ByteBuf* self = (ByteBuf*)vp; (void)self;
}
#endif

#ifndef Reloc_STRUCT_DEFINED
#define Reloc_STRUCT_DEFINED
typedef struct Reloc {
    size_t __rc;
    long long offset;
    TrStr symbol;
    long long kind;
    long long str_idx;
} Reloc;
static void _trdrop_Reloc(void* vp) {
    Reloc* self = (Reloc*)vp; (void)self;
    _tr_str_release(self->symbol);
}
#endif

#ifndef Jump_STRUCT_DEFINED
#define Jump_STRUCT_DEFINED
typedef struct Jump {
    size_t __rc;
    long long patch_off;
    long long target;
} Jump;
static void _trdrop_Jump(void* vp) {
    Jump* self = (Jump*)vp; (void)self;
}
#endif

#ifndef EncodedFunc_STRUCT_DEFINED
#define EncodedFunc_STRUCT_DEFINED
typedef struct EncodedFunc {
    size_t __rc;
    TrStr name;
    bool is_main;
    ByteBuf* code;
    List_ptr* relocs;
} EncodedFunc;
static void _trdrop_EncodedFunc(void* vp) {
    EncodedFunc* self = (EncodedFunc*)vp; (void)self;
    _tr_str_release(self->name);
    _tr_obj_release(self->code, _trdrop_ByteBuf);
    List_ptr_free_obj(self->relocs, _trdrop_Reloc);
}
#endif

#ifndef TextReloc_STRUCT_DEFINED
#define TextReloc_STRUCT_DEFINED
typedef struct TextReloc {
    size_t __rc;
    long long roff;
    TrStr sym;
    long long kind;
    long long str_idx;
} TextReloc;
static void _trdrop_TextReloc(void* vp) {
    TextReloc* self = (TextReloc*)vp; (void)self;
    _tr_str_release(self->sym);
}
#endif

#ifndef NativeGenerator_STRUCT_DEFINED
#define NativeGenerator_STRUCT_DEFINED
typedef struct NativeGenerator {
    size_t __rc;
    TrStr target;
    bool ready;
    TrStr fail_note;
} NativeGenerator;
static void _trdrop_NativeGenerator(void* vp) {
    NativeGenerator* self = (NativeGenerator*)vp; (void)self;
    _tr_str_release(self->target);
    _tr_str_release(self->fail_note);
}
#endif

#ifndef GVal_STRUCT_DEFINED
#define GVal_STRUCT_DEFINED
typedef struct GVal {
    size_t __rc;
    TrStr ty;
    TrStr val;
} GVal;
static void _trdrop_GVal(void* vp) {
    GVal* self = (GVal*)vp; (void)self;
    _tr_str_release(self->ty);
    _tr_str_release(self->val);
}
#endif

#ifndef GpuEmitter_STRUCT_DEFINED
#define GpuEmitter_STRUCT_DEFINED
typedef struct GpuEmitter {
    size_t __rc;
    StringBuilder* sb;
    TrStr target;
    long long tmp;
    long long lbl;
    bool ok;
    TrStr fail_note;
    List_TrStr* var_names;
    List_TrStr* var_ll;
    List_TrStr* var_elem;
    TrStr ret_llty;
    List_ptr* dev_fns;
} GpuEmitter;
static void _trdrop_GpuEmitter(void* vp) {
    GpuEmitter* self = (GpuEmitter*)vp; (void)self;
    _tr_str_release(self->target);
    _tr_str_release(self->fail_note);
    List_TrStr_free(self->var_names);
    List_TrStr_free(self->var_ll);
    List_TrStr_free(self->var_elem);
    _tr_str_release(self->ret_llty);
}
#endif

#ifndef GpuGenerator_STRUCT_DEFINED
#define GpuGenerator_STRUCT_DEFINED
typedef struct GpuGenerator {
    size_t __rc;
    bool ok;
    TrStr fail_note;
    long long n_kernels;
} GpuGenerator;
static void _trdrop_GpuGenerator(void* vp) {
    GpuGenerator* self = (GpuGenerator*)vp; (void)self;
    _tr_str_release(self->fail_note);
}
#endif

#ifndef MacroCtx_STRUCT_DEFINED
#define MacroCtx_STRUCT_DEFINED
typedef struct MacroCtx {
    size_t __rc;
    TrMap* env;
    bool returned;
    TrStr result;
    bool has_error;
    TrStr error_msg;
    long long gensym_ctr;
} MacroCtx;
static void _trdrop_MacroCtx(void* vp) {
    MacroCtx* self = (MacroCtx*)vp; (void)self;
    Dict_free(self->env);
    _tr_str_release(self->result);
    _tr_str_release(self->error_msg);
}
#endif

#ifndef FnMacroExpander_STRUCT_DEFINED
#define FnMacroExpander_STRUCT_DEFINED
typedef struct FnMacroExpander {
    size_t __rc;
    TrMap* macros;
    long long errors;
    long long depth;
    long long gensym_base;
} FnMacroExpander;
static void _trdrop_FnMacroExpander(void* vp) {
    FnMacroExpander* self = (FnMacroExpander*)vp; (void)self;
}
#endif

typedef struct List_Token { Token* data; size_t len; size_t capacity; } List_Token;
static inline List_Token* List_Token_new(void) { List_Token* l=(List_Token*)malloc(sizeof(List_Token)); l->data=(Token*)malloc(sizeof(Token)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Token_append(List_Token* l, Token val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Token*)realloc(l->data,sizeof(Token)*l->capacity); } l->data[l->len++]=val; }
static inline void List_Token_reserve(List_Token* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(Token*)realloc(l->data,sizeof(Token)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline Token List_Token_get(List_Token* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline Token List_Token_pop(List_Token* l) { if(!l||l->len==0) return (Token){0}; l->len--; return l->data[l->len]; }
static inline void List_Token_free(List_Token* l) { if(l){ free(l->data); free(l); } }
typedef struct List_Pattern { Pattern* data; size_t len; size_t capacity; } List_Pattern;
static inline List_Pattern* List_Pattern_new(void) { List_Pattern* l=(List_Pattern*)malloc(sizeof(List_Pattern)); l->data=(Pattern*)malloc(sizeof(Pattern)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Pattern_append(List_Pattern* l, Pattern val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Pattern*)realloc(l->data,sizeof(Pattern)*l->capacity); } l->data[l->len++]=val; }
static inline void List_Pattern_reserve(List_Pattern* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(Pattern*)realloc(l->data,sizeof(Pattern)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline Pattern List_Pattern_get(List_Pattern* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline Pattern List_Pattern_pop(List_Pattern* l) { if(!l||l->len==0) return (Pattern){0}; l->len--; return l->data[l->len]; }
static inline void List_Pattern_free(List_Pattern* l) { if(l){ free(l->data); free(l); } }
typedef struct List_Ownership { Ownership* data; size_t len; size_t capacity; } List_Ownership;
static inline List_Ownership* List_Ownership_new(void) { List_Ownership* l=(List_Ownership*)malloc(sizeof(List_Ownership)); l->data=(Ownership*)malloc(sizeof(Ownership)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Ownership_append(List_Ownership* l, Ownership val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Ownership*)realloc(l->data,sizeof(Ownership)*l->capacity); } l->data[l->len++]=val; }
static inline void List_Ownership_reserve(List_Ownership* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(Ownership*)realloc(l->data,sizeof(Ownership)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline Ownership List_Ownership_get(List_Ownership* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline Ownership List_Ownership_pop(List_Ownership* l) { if(!l||l->len==0) return (Ownership){0}; l->len--; return l->data[l->len]; }
static inline void List_Ownership_free(List_Ownership* l) { if(l){ free(l->data); free(l); } }
typedef struct List_Expr { Expr* data; size_t len; size_t capacity; } List_Expr;
static inline List_Expr* List_Expr_new(void) { List_Expr* l=(List_Expr*)malloc(sizeof(List_Expr)); l->data=(Expr*)malloc(sizeof(Expr)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Expr_append(List_Expr* l, Expr val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Expr*)realloc(l->data,sizeof(Expr)*l->capacity); } l->data[l->len++]=val; }
static inline void List_Expr_reserve(List_Expr* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(Expr*)realloc(l->data,sizeof(Expr)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline Expr List_Expr_get(List_Expr* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline Expr List_Expr_pop(List_Expr* l) { if(!l||l->len==0) return (Expr){0}; l->len--; return l->data[l->len]; }
static inline void List_Expr_free(List_Expr* l) { if(l){ free(l->data); free(l); } }
typedef struct List_Stmt { Stmt* data; size_t len; size_t capacity; } List_Stmt;
static inline List_Stmt* List_Stmt_new(void) { List_Stmt* l=(List_Stmt*)malloc(sizeof(List_Stmt)); l->data=(Stmt*)malloc(sizeof(Stmt)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Stmt_append(List_Stmt* l, Stmt val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Stmt*)realloc(l->data,sizeof(Stmt)*l->capacity); } l->data[l->len++]=val; }
static inline void List_Stmt_reserve(List_Stmt* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(Stmt*)realloc(l->data,sizeof(Stmt)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline Stmt List_Stmt_get(List_Stmt* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline Stmt List_Stmt_pop(List_Stmt* l) { if(!l||l->len==0) return (Stmt){0}; l->len--; return l->data[l->len]; }
static inline void List_Stmt_free(List_Stmt* l) { if(l){ free(l->data); free(l); } }
typedef struct List_Decl { Decl* data; size_t len; size_t capacity; } List_Decl;
static inline List_Decl* List_Decl_new(void) { List_Decl* l=(List_Decl*)malloc(sizeof(List_Decl)); l->data=(Decl*)malloc(sizeof(Decl)*8); l->len=0; l->capacity=8; return l; }
static inline void List_Decl_append(List_Decl* l, Decl val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(Decl*)realloc(l->data,sizeof(Decl)*l->capacity); } l->data[l->len++]=val; }
static inline void List_Decl_reserve(List_Decl* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(Decl*)realloc(l->data,sizeof(Decl)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline Decl List_Decl_get(List_Decl* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline Decl List_Decl_pop(List_Decl* l) { if(!l||l->len==0) return (Decl){0}; l->len--; return l->data[l->len]; }
static inline void List_Decl_free(List_Decl* l) { if(l){ free(l->data); free(l); } }
typedef struct List_HirExpr { HirExpr* data; size_t len; size_t capacity; } List_HirExpr;
static inline List_HirExpr* List_HirExpr_new(void) { List_HirExpr* l=(List_HirExpr*)malloc(sizeof(List_HirExpr)); l->data=(HirExpr*)malloc(sizeof(HirExpr)*8); l->len=0; l->capacity=8; return l; }
static inline void List_HirExpr_append(List_HirExpr* l, HirExpr val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(HirExpr*)realloc(l->data,sizeof(HirExpr)*l->capacity); } l->data[l->len++]=val; }
static inline void List_HirExpr_reserve(List_HirExpr* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(HirExpr*)realloc(l->data,sizeof(HirExpr)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline HirExpr List_HirExpr_get(List_HirExpr* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline HirExpr List_HirExpr_pop(List_HirExpr* l) { if(!l||l->len==0) return (HirExpr){0}; l->len--; return l->data[l->len]; }
static inline void List_HirExpr_free(List_HirExpr* l) { if(l){ free(l->data); free(l); } }
typedef struct List_HirStmt { HirStmt* data; size_t len; size_t capacity; } List_HirStmt;
static inline List_HirStmt* List_HirStmt_new(void) { List_HirStmt* l=(List_HirStmt*)malloc(sizeof(List_HirStmt)); l->data=(HirStmt*)malloc(sizeof(HirStmt)*8); l->len=0; l->capacity=8; return l; }
static inline void List_HirStmt_append(List_HirStmt* l, HirStmt val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(HirStmt*)realloc(l->data,sizeof(HirStmt)*l->capacity); } l->data[l->len++]=val; }
static inline void List_HirStmt_reserve(List_HirStmt* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(HirStmt*)realloc(l->data,sizeof(HirStmt)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline HirStmt List_HirStmt_get(List_HirStmt* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline HirStmt List_HirStmt_pop(List_HirStmt* l) { if(!l||l->len==0) return (HirStmt){0}; l->len--; return l->data[l->len]; }
static inline void List_HirStmt_free(List_HirStmt* l) { if(l){ free(l->data); free(l); } }
typedef struct List_MirStmt { MirStmt* data; size_t len; size_t capacity; } List_MirStmt;
static inline List_MirStmt* List_MirStmt_new(void) { List_MirStmt* l=(List_MirStmt*)malloc(sizeof(List_MirStmt)); l->data=(MirStmt*)malloc(sizeof(MirStmt)*8); l->len=0; l->capacity=8; return l; }
static inline void List_MirStmt_append(List_MirStmt* l, MirStmt val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(MirStmt*)realloc(l->data,sizeof(MirStmt)*l->capacity); } l->data[l->len++]=val; }
static inline void List_MirStmt_reserve(List_MirStmt* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(MirStmt*)realloc(l->data,sizeof(MirStmt)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline MirStmt List_MirStmt_get(List_MirStmt* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline MirStmt List_MirStmt_pop(List_MirStmt* l) { if(!l||l->len==0) return (MirStmt){0}; l->len--; return l->data[l->len]; }
static inline void List_MirStmt_free(List_MirStmt* l) { if(l){ free(l->data); free(l); } }
typedef struct List_MirTerm { MirTerm* data; size_t len; size_t capacity; } List_MirTerm;
static inline List_MirTerm* List_MirTerm_new(void) { List_MirTerm* l=(List_MirTerm*)malloc(sizeof(List_MirTerm)); l->data=(MirTerm*)malloc(sizeof(MirTerm)*8); l->len=0; l->capacity=8; return l; }
static inline void List_MirTerm_append(List_MirTerm* l, MirTerm val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(MirTerm*)realloc(l->data,sizeof(MirTerm)*l->capacity); } l->data[l->len++]=val; }
static inline void List_MirTerm_reserve(List_MirTerm* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(MirTerm*)realloc(l->data,sizeof(MirTerm)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline MirTerm List_MirTerm_get(List_MirTerm* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline MirTerm List_MirTerm_pop(List_MirTerm* l) { if(!l||l->len==0) return (MirTerm){0}; l->len--; return l->data[l->len]; }
static inline void List_MirTerm_free(List_MirTerm* l) { if(l){ free(l->data); free(l); } }
typedef struct List_SymbolKind { SymbolKind* data; size_t len; size_t capacity; } List_SymbolKind;
static inline List_SymbolKind* List_SymbolKind_new(void) { List_SymbolKind* l=(List_SymbolKind*)malloc(sizeof(List_SymbolKind)); l->data=(SymbolKind*)malloc(sizeof(SymbolKind)*8); l->len=0; l->capacity=8; return l; }
static inline void List_SymbolKind_append(List_SymbolKind* l, SymbolKind val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(SymbolKind*)realloc(l->data,sizeof(SymbolKind)*l->capacity); } l->data[l->len++]=val; }
static inline void List_SymbolKind_reserve(List_SymbolKind* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(SymbolKind*)realloc(l->data,sizeof(SymbolKind)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline SymbolKind List_SymbolKind_get(List_SymbolKind* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline SymbolKind List_SymbolKind_pop(List_SymbolKind* l) { if(!l||l->len==0) return (SymbolKind){0}; l->len--; return l->data[l->len]; }
static inline void List_SymbolKind_free(List_SymbolKind* l) { if(l){ free(l->data); free(l); } }
typedef struct List_LType { LType* data; size_t len; size_t capacity; } List_LType;
static inline List_LType* List_LType_new(void) { List_LType* l=(List_LType*)malloc(sizeof(List_LType)); l->data=(LType*)malloc(sizeof(LType)*8); l->len=0; l->capacity=8; return l; }
static inline void List_LType_append(List_LType* l, LType val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(LType*)realloc(l->data,sizeof(LType)*l->capacity); } l->data[l->len++]=val; }
static inline void List_LType_reserve(List_LType* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(LType*)realloc(l->data,sizeof(LType)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline LType List_LType_get(List_LType* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline LType List_LType_pop(List_LType* l) { if(!l||l->len==0) return (LType){0}; l->len--; return l->data[l->len]; }
static inline void List_LType_free(List_LType* l) { if(l){ free(l->data); free(l); } }
typedef struct List_LInst { LInst* data; size_t len; size_t capacity; } List_LInst;
static inline List_LInst* List_LInst_new(void) { List_LInst* l=(List_LInst*)malloc(sizeof(List_LInst)); l->data=(LInst*)malloc(sizeof(LInst)*8); l->len=0; l->capacity=8; return l; }
static inline void List_LInst_append(List_LInst* l, LInst val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(LInst*)realloc(l->data,sizeof(LInst)*l->capacity); } l->data[l->len++]=val; }
static inline void List_LInst_reserve(List_LInst* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(LInst*)realloc(l->data,sizeof(LInst)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline LInst List_LInst_get(List_LInst* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline LInst List_LInst_pop(List_LInst* l) { if(!l||l->len==0) return (LInst){0}; l->len--; return l->data[l->len]; }
static inline void List_LInst_free(List_LInst* l) { if(l){ free(l->data); free(l); } }
typedef struct List_LTerm { LTerm* data; size_t len; size_t capacity; } List_LTerm;
static inline List_LTerm* List_LTerm_new(void) { List_LTerm* l=(List_LTerm*)malloc(sizeof(List_LTerm)); l->data=(LTerm*)malloc(sizeof(LTerm)*8); l->len=0; l->capacity=8; return l; }
static inline void List_LTerm_append(List_LTerm* l, LTerm val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(LTerm*)realloc(l->data,sizeof(LTerm)*l->capacity); } l->data[l->len++]=val; }
static inline void List_LTerm_reserve(List_LTerm* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(LTerm*)realloc(l->data,sizeof(LTerm)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline LTerm List_LTerm_get(List_LTerm* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline LTerm List_LTerm_pop(List_LTerm* l) { if(!l||l->len==0) return (LTerm){0}; l->len--; return l->data[l->len]; }
static inline void List_LTerm_free(List_LTerm* l) { if(l){ free(l->data); free(l); } }
typedef struct List_MacroVal { MacroVal* data; size_t len; size_t capacity; } List_MacroVal;
static inline List_MacroVal* List_MacroVal_new(void) { List_MacroVal* l=(List_MacroVal*)malloc(sizeof(List_MacroVal)); l->data=(MacroVal*)malloc(sizeof(MacroVal)*8); l->len=0; l->capacity=8; return l; }
static inline void List_MacroVal_append(List_MacroVal* l, MacroVal val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(MacroVal*)realloc(l->data,sizeof(MacroVal)*l->capacity); } l->data[l->len++]=val; }
static inline void List_MacroVal_reserve(List_MacroVal* l, long long cap) { if(l && (size_t)cap > l->capacity){ l->data=(MacroVal*)realloc(l->data,sizeof(MacroVal)*(size_t)cap); l->capacity=(size_t)cap; } }
static inline MacroVal List_MacroVal_get(List_MacroVal* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline MacroVal List_MacroVal_pop(List_MacroVal* l) { if(!l||l->len==0) return (MacroVal){0}; l->len--; return l->data[l->len]; }
static inline void List_MacroVal_free(List_MacroVal* l) { if(l){ free(l->data); free(l); } }

__attribute__((hot)) TrStr read_file(TrStr path);
__attribute__((hot)) bool file_exists(TrStr path);
__attribute__((hot)) bool write_file(TrStr path, TrStr content);
__attribute__((hot)) bool append_file(TrStr path, TrStr content);
__attribute__((malloc,returns_nonnull,hot)) StringObj* StringObj_init(TrStr s);
__attribute__((hot)) TrStr StringObj_as_str(StringObj* self);
__attribute__((hot)) void StringObj_append(StringObj* self, TrStr other);
__attribute__((hot)) void StringObj_destroy(StringObj* self);
__attribute__((malloc,returns_nonnull,hot)) StringBuilder* StringBuilder_init(long long initial_capacity);
__attribute__((hot)) void StringBuilder_append(StringBuilder* self, TrStr s);
__attribute__((hot)) void StringBuilder_append_char(StringBuilder* self, long long c);
__attribute__((hot)) void StringBuilder_append_int(StringBuilder* self, long long n);
__attribute__((hot)) void StringBuilder_append_float(StringBuilder* self, double f);
__attribute__((hot)) long long StringBuilder_len(StringBuilder* self);
__attribute__((hot)) StringObj* StringBuilder_to_string(StringBuilder* self);
__attribute__((hot)) TrStr StringBuilder_to_owned(StringBuilder* self);
__attribute__((hot)) TrStr StringBuilder_as_str(StringBuilder* self);
__attribute__((hot)) void StringBuilder_clear(StringBuilder* self);
__attribute__((hot)) void StringBuilder__tr_fn_free(StringBuilder* self);
__attribute__((hot)) bool Token_is_eof(Token self);
__attribute__((hot)) bool Token_is_newline(Token self);
__attribute__((hot)) bool Token_is_keyword(Token self);
__attribute__((hot)) TrStr Token_debug(Token self);
__attribute__((hot)) Token keyword_to_token(TrStr s);
__attribute__((hot)) bool char_is_digit(long long c);
__attribute__((hot)) bool char_is_alpha(long long c);
__attribute__((hot)) bool char_is_alnum(long long c);
__attribute__((hot)) bool char_is_space(long long c);
__attribute__((hot)) bool char_is_newline(long long c);
__attribute__((hot)) bool char_is_hex(long long c);
__attribute__((hot)) bool _peek_next_line_dot(char* src, long long pos, long long src_len);
__attribute__((hot)) bool _last_tok_is_dot(List_Token* tokens);
__attribute__((hot)) char* raw_alloc(long long size);
__attribute__((hot)) char* raw_realloc(char* ptr, long long size);
__attribute__((hot)) void raw_free(char* ptr);
__attribute__((hot)) void raw_copy(char* dst, char* src, long long n);
__attribute__((hot)) void raw_zero(char* ptr, long long n);
__attribute__((hot)) void raw_move(char* dst, char* src, long long n);
__attribute__((hot)) bool color_enabled();
__attribute__((hot)) TrStr esc();
__attribute__((hot)) TrStr paint(TrStr s, TrStr code);
__attribute__((hot)) TrStr c_red(TrStr s);
__attribute__((hot)) TrStr c_yellow(TrStr s);
__attribute__((hot)) TrStr c_green(TrStr s);
__attribute__((hot)) TrStr c_cyan(TrStr s);
__attribute__((hot)) TrStr c_dim(TrStr s);
__attribute__((hot)) TrStr c_bold(TrStr s);
__attribute__((hot)) TrStr spaces(long long n);
__attribute__((hot)) TrStr repeat_char(TrStr ch, long long n);
__attribute__((hot)) TrStr first_quoted(TrStr msg);
__attribute__((hot)) long long col_of(TrStr line, TrStr needle);
__attribute__((hot)) TrStr loc_file(TrStr head);
__attribute__((hot)) long long loc_line(TrStr head);
__attribute__((malloc,returns_nonnull,hot)) AstType* AstType_init(TrStr name);
__attribute__((hot)) AstType* AstType_init_generic(TrStr name, AstType** arg);
__attribute__((malloc,returns_nonnull,hot)) GenericConstraint* GenericConstraint_init(TrStr target);
__attribute__((malloc,returns_nonnull,hot)) Decorator* Decorator_init(TrStr name);
__attribute__((malloc,returns_nonnull,hot)) Comprehension* Comprehension_init(TrStr target, Expr* iter);
__attribute__((malloc,returns_nonnull,hot)) CatchClause* CatchClause_init(TrStr err_name, Block** body);
__attribute__((malloc,returns_nonnull,hot)) MatchArm* MatchArm_init(Pattern pat, Block** body);
__attribute__((hot)) FStringPart* FStringPart_init_text(TrStr s);
__attribute__((hot)) FStringPart* FStringPart_init_expr(Expr* e);
__attribute__((hot)) FStringPart* FStringPart_init_expr_fmt(Expr* e, TrStr spec);
__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_recv(Expr* chan, TrStr var, Block* body);
__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_send(Expr* chan, Expr* val, Block* body);
__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_timeout(Expr* ms, Block* body);
__attribute__((hot)) ChanSelectArm* ChanSelectArm_init_default(Block* body);
__attribute__((malloc,returns_nonnull,hot)) Block* Block_init();
__attribute__((hot)) void Block_push(Block* self, Stmt* s);
__attribute__((hot)) long long Block_len(Block* self);
__attribute__((hot)) Stmt* Block_get(Block* self, long long i);
__attribute__((malloc,returns_nonnull,hot)) ElifClause* ElifClause_init(Expr* cond, Block** body);
__attribute__((malloc,returns_nonnull,hot)) Param* Param_init(TrStr name, AstType** ty);
__attribute__((malloc,returns_nonnull,hot)) FunctionDef* FunctionDef_init(TrStr name);
__attribute__((malloc,returns_nonnull,hot)) FieldDef* FieldDef_init(TrStr name, AstType** ty);
__attribute__((malloc,returns_nonnull,hot)) ClassDef* ClassDef_init(TrStr name);
__attribute__((malloc,returns_nonnull,hot)) VariantDef* VariantDef_init(TrStr name);
__attribute__((malloc,returns_nonnull,hot)) EnumDef* EnumDef_init(TrStr name);
__attribute__((malloc,returns_nonnull,hot)) InterfaceDef* InterfaceDef_init(TrStr name);
__attribute__((malloc,returns_nonnull,hot)) ImportItem* ImportItem_init(TrStr name);
__attribute__((malloc,returns_nonnull,hot)) Program* Program_init();
__attribute__((hot)) void Program_push(Program* self, Decl* d);
__attribute__((hot)) long long Program_len(Program* self);
__attribute__((hot)) Decl* Program_get(Program* self, long long i);
__attribute__((hot)) void _dummy_instantiations();
__attribute__((hot)) Expr* box_expr(Expr e);
__attribute__((hot)) Stmt* box_stmt(Stmt s);
__attribute__((hot)) Decl* box_decl(Decl d);
__attribute__((hot)) AstType** box_asttype(AstType* t);
__attribute__((hot)) TrStr _nth_source_line(TrStr src, long long n);
__attribute__((hot)) TrStr _spaces(long long n);
__attribute__((malloc,returns_nonnull,hot)) Parser* Parser_init(List_Token* tokens, List_i64* lines);
__attribute__((hot)) Token Parser_peek(Parser* self);
__attribute__((hot)) Token Parser_advance(Parser* self);
__attribute__((hot)) void Parser_skip_newlines(Parser* self);
__attribute__((hot)) void Parser_skip_newlines_and_indent(Parser* self);
__attribute__((hot)) void Parser_expect_newline(Parser* self);
__attribute__((hot)) bool Parser_at_end(Parser* self);
__attribute__((hot)) long long Parser_cur_line(Parser* self);
__attribute__((hot)) long long Parser_cur_col(Parser* self);
__attribute__((hot)) void Parser_emit_diag(Parser* self, TrStr msg, TrStr hint);
__attribute__((hot)) TrStr Parser_consume_ident(Parser* self);
__attribute__((hot)) TrStr Parser_consume_module_ident(Parser* self);
__attribute__((hot)) AstType* Parser_parse_type(Parser* self);
__attribute__((hot)) List_ptr* Parser_parse_param_list(Parser* self);
__attribute__((hot)) Block* Parser_parse_block(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_try_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_assert_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_with_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_asm_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_spawn_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_taskgroup_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_chan_select_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_gpu_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_let_stmt(Parser* self, bool is_mut);
__attribute__((hot)) Stmt* Parser_parse_shared_let_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_const_let_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_if_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_while_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_loop_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_for_stmt(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_match_stmt(Parser* self);
__attribute__((hot)) List_ptr* Parser_parse_match_arms(Parser* self);
__attribute__((hot)) Pattern Parser_parse_pattern(Parser* self);
__attribute__((hot)) Stmt* Parser_parse_assign_or_expr_stmt(Parser* self);
__attribute__((hot)) Expr* Parser_parse_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_match_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_if_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_ternary(Parser* self);
__attribute__((hot)) Expr* Parser_parse_or_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_and_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_not_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_comparison(Parser* self);
__attribute__((hot)) TrStr Parser__peek_cmp_op(Parser* self);
__attribute__((hot)) Expr* Parser_parse_bitor_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_bitxor_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_bitand_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_shift_expr(Parser* self);
__attribute__((hot)) Expr* Parser_parse_additive(Parser* self);
__attribute__((hot)) Expr* Parser_parse_multiplicative(Parser* self);
__attribute__((hot)) Expr* Parser_parse_power(Parser* self);
__attribute__((hot)) Expr* Parser_parse_unary(Parser* self);
__attribute__((hot)) Expr* Parser_parse_postfix(Parser* self);
__attribute__((hot)) void Parser_emit_diag_at(Parser* self, long long ln, long long col, TrStr msg, TrStr hint);
__attribute__((hot)) void Parser_expect_rparen(Parser* self, long long oln, long long ocol, TrStr what);
__attribute__((hot)) void Parser_expect_rbracket(Parser* self, long long oln, long long ocol, TrStr what);
__attribute__((hot)) void Parser_expect_rbrace(Parser* self, long long oln, long long ocol, TrStr what);
__attribute__((hot)) List_ptr* Parser_parse_arg_list(Parser* self);
__attribute__((hot)) Expr* Parser_parse_primary(Parser* self);
__attribute__((hot)) Expr* Parser_parse_fstring(Parser* self, TrStr raw);
__attribute__((hot)) Program* Parser_parse_program(Parser* self);
__attribute__((hot)) Decl* Parser_parse_decl(Parser* self);
__attribute__((hot)) Decl* Parser_parse_from_import(Parser* self);
__attribute__((hot)) Decl* Parser_parse_import(Parser* self);
__attribute__((hot)) void Parser_parse_generic_bound(Parser* self, TrStr gname, List_ptr* constraints);
__attribute__((hot)) FunctionDef* Parser_parse_function_def(Parser* self, bool is_method);
__attribute__((hot)) Decl* Parser_parse_class_decl(Parser* self);
__attribute__((hot)) Decl* Parser_parse_enum_decl(Parser* self);
__attribute__((hot)) Decl* Parser_parse_interface_decl(Parser* self);
__attribute__((hot)) Decl* Parser_parse_extend_decl(Parser* self);
__attribute__((hot)) Decl* Parser_parse_extern_decl(Parser* self);
__attribute__((hot)) long long _find_fmt_colon(TrStr s);
__attribute__((hot)) bool decl_is_pub(Decl d);
__attribute__((malloc,returns_nonnull,hot)) ModuleResolver* ModuleResolver_init();
__attribute__((hot)) void ModuleResolver_add_search_path(ModuleResolver* self, TrStr p);
__attribute__((hot)) TrStr ModuleResolver_dir_of_path(ModuleResolver* self, TrStr path);
__attribute__((hot)) TrStr ModuleResolver_base_of_path(ModuleResolver* self, TrStr path);
__attribute__((hot)) Program* ModuleResolver_resolve_main(ModuleResolver* self, TrStr main_path);
__attribute__((hot)) void ModuleResolver_resolve_file(ModuleResolver* self, TrStr path, bool is_root);
__attribute__((hot)) bool ModuleResolver__path_is_lib(ModuleResolver* self, TrStr path);
__attribute__((hot)) bool ModuleResolver__source_is_trusted(ModuleResolver* self, TrStr source);
__attribute__((hot)) bool ModuleResolver__contains(ModuleResolver* self, TrStr hay, TrStr needle);
__attribute__((hot)) void ModuleResolver__mark_decl_lib(ModuleResolver* self, Decl* dp);
__attribute__((hot)) void ModuleResolver__mark_methods_lib(ModuleResolver* self, ClassDef* c);
__attribute__((hot)) void ModuleResolver_resolve_recursive(ModuleResolver* self, TrStr path);
__attribute__((hot)) void ModuleResolver_resolve_module_path(ModuleResolver* self, TrStr mod_path);
__attribute__((malloc,returns_nonnull,hot)) HirFStringPart* HirFStringPart_init();
__attribute__((malloc,returns_nonnull,hot)) HirMatchArm* HirMatchArm_init(Pattern pat, HirBlock* body);
__attribute__((malloc,returns_nonnull,hot)) HirBlock* HirBlock_init();
__attribute__((hot)) void HirBlock_push(HirBlock* self, HirStmt* s);
__attribute__((malloc,returns_nonnull,hot)) HirProgram* HirProgram_init();
__attribute__((hot)) HirExpr* box_hirexpr(HirExpr e);
__attribute__((hot)) HirStmt* box_hirstmt(HirStmt s);
__attribute__((hot)) AstType* hir_expr_type(HirExpr* e);
__attribute__((hot)) long long _tr_str_len(TrStr s);
__attribute__((hot)) bool _is_mutating_call_on(HirExpr* val, TrStr source, TrMap* mm);
__attribute__((hot)) MirStmt* box_mirstmt(MirStmt s);
__attribute__((hot)) MirTerm* box_mirterm(MirTerm t);
__attribute__((malloc,returns_nonnull,hot)) MirBuilder* MirBuilder_init();
__attribute__((hot)) void MirBuilder_record_borrow(MirBuilder* self, TrStr borrower, TrStr source, bool exclusive);
__attribute__((hot)) void MirBuilder_record_coll_borrow(MirBuilder* self, TrStr borrower, TrStr source, bool exclusive);
__attribute__((hot)) long long MirBuilder_new_block(MirBuilder* self);
__attribute__((hot)) void MirBuilder_push_stmt(MirBuilder* self, MirStmt s);
__attribute__((hot)) void MirBuilder_set_term(MirBuilder* self, MirTerm t);
__attribute__((hot)) bool MirBuilder_terminated(MirBuilder* self);
__attribute__((malloc,returns_nonnull,hot)) MirProgram* MirProgram_init();
__attribute__((hot)) void lower_stmts(MirBuilder* b, HirBlock* hb);
__attribute__((hot)) MirFunction* lower_function(HirFunction* hf);
__attribute__((hot)) MirProgram* lower_program(HirProgram* hir);
__attribute__((hot)) bool set_contains(List_TrStr* v, TrStr s);
__attribute__((hot)) bool set_add(List_TrStr* v, TrStr s);
__attribute__((hot)) void collect_uses(HirExpr* e, List_TrStr* out);
__attribute__((hot)) void collect_raw_borrows(HirExpr* e, LiveSet* out);
__attribute__((hot)) void add_exposed(HirExpr* e, List_TrStr* gen, List_TrStr* kill);
__attribute__((hot)) void block_use_def(MirBlock* blk, List_TrStr* gen, List_TrStr* kill);
__attribute__((hot)) void block_succs(MirBlock* blk, List_i64* out);
__attribute__((malloc,returns_nonnull,hot)) LiveSet* LiveSet_init();
__attribute__((hot)) bool LiveSet_has(LiveSet* self, TrStr s);
__attribute__((hot)) bool LiveSet_add(LiveSet* self, TrStr s);
__attribute__((hot)) bool LiveSet_union_in(LiveSet* self, LiveSet* other);
__attribute__((hot)) bool LiveSet_union_except(LiveSet* self, LiveSet* src, LiveSet* block);
__attribute__((hot)) LiveSet* LiveSet_clone(LiveSet* self);
__attribute__((hot)) void LiveSet_retain_common(LiveSet* self, LiveSet* other);
__attribute__((hot)) bool LiveSet_equals(LiveSet* self, LiveSet* other);
__attribute__((hot)) void LiveSet_set_to(LiveSet* self, LiveSet* other);
__attribute__((hot)) LiveSet* LiveSet_minus(LiveSet* self, LiveSet* other);
__attribute__((hot)) List_ptr* compute_liveness(MirFunction* mf);
__attribute__((hot)) void collect_moves(HirExpr* e, List_TrStr* out);
__attribute__((hot)) bool is_free_fn(TrStr n);
__attribute__((hot)) TrStr free_target(HirExpr* arg);
__attribute__((hot)) void collect_frees(HirExpr* e, List_TrStr* out);
__attribute__((hot)) void block_gen_own(MirBlock* blk, List_TrStr* out);
__attribute__((hot)) void block_moves(MirBlock* blk, List_TrStr* out);
__attribute__((hot)) void preds_of(MirFunction* mf, long long b, List_i64* out);
__attribute__((hot)) List_ptr* compute_drops(MirFunction* mf, List_ptr* live_out);
__attribute__((hot)) long long last_use_in_block(MirBlock* blk, TrStr name);
__attribute__((hot)) List_bool* compute_borrow_outlives(MirFunction* mf, List_ptr* live_out);
__attribute__((hot)) bool is_if_body(MirFunction* mf, HirBlock* hb);
__attribute__((hot)) DropSite* site_for(List_ptr* out, HirBlock* hb);
__attribute__((hot)) List_TrStr* mir_proven_borrows(HirFunction* hf);
__attribute__((hot)) bool borrower_live_after(MirBlock* blk, long long after_idx, TrStr name, LiveSet* lo);
__attribute__((hot)) List_TrStr* mir_borrow_conflicts(HirFunction* hf, TrMap* mutating_methods);
__attribute__((hot)) List_TrStr* mir_shared_ref_param_violations(HirFunction* hf, TrMap* mutating_methods);
__attribute__((hot)) List_ptr* mir_if_drop_plan(HirFunction* hf);
__attribute__((hot)) TrStr set_str(List_TrStr* v);
__attribute__((hot)) TrStr term_str(MirTerm* t);
__attribute__((hot)) TrStr stmt_str(MirStmt* s);
__attribute__((hot)) TrStr dump_mir(MirProgram* mp);
__attribute__((malloc,returns_nonnull,hot)) Symbol* Symbol_init(TrStr name, SymbolKind kind, AstType** ty);
__attribute__((malloc,returns_nonnull,hot)) Scope* Scope_init();
__attribute__((hot)) bool _expr_is_self_field(Expr* e);
__attribute__((hot)) bool _binop_is_float_name(TrStr n);
__attribute__((hot)) bool _hl_has_float(List_ptr* hl);
__attribute__((hot)) bool _block_mutates_self(Block* b);
__attribute__((hot)) bool _pblock_mutates_self(Block** pb);
__attribute__((hot)) bool _stmt_mutates_self(Stmt* s);
__attribute__((hot)) AstType** Sema_build_ast_type(Sema* self, Expr* e);
__attribute__((hot)) AstType** Sema__targ_of(Sema* self, Expr* e);
__attribute__((malloc,returns_nonnull,hot)) Sema* Sema_init();
__attribute__((hot)) TrStr Sema_io_ty_str(Sema* self, AstType* ty);
__attribute__((hot)) TrStr Sema_io_doc_of(Sema* self, Block* body);
__attribute__((hot)) TrStr Sema_io_func_sig(Sema* self, FunctionDef* f);
__attribute__((hot)) TrStr Sema_build_inspect_str(Sema* self, TrStr name);
__attribute__((hot)) void Sema_error(Sema* self, TrStr msg);
__attribute__((hot)) bool Sema_is_sendable_type(Sema* self, TrStr ty_name);
__attribute__((hot)) bool Sema_is_sendable_ty(Sema* self, AstType* ty);
__attribute__((hot)) bool Sema_class_method_exists(Sema* self, TrStr cls_name, TrStr method);
__attribute__((hot)) bool Sema_is_universal_method(Sema* self, TrStr method);
__attribute__((hot)) bool Sema_expr_is_borrow(Sema* self, HirExpr* e);
__attribute__((hot)) bool Sema__expr_is_shared(Sema* self, HirExpr* e);
__attribute__((hot)) void Sema_check_spawn_sendable(Sema* self, HirExpr* e);
__attribute__((hot)) void Sema_check_class_sendable_fields(Sema* self, ClassDef* c);
__attribute__((hot)) HirExpr* Sema__patch_empty_dict_hint(Sema* self, HirExpr* hv, AstType* hint_ty);
__attribute__((hot)) void Sema_mark_moved(Sema* self, TrStr name);
__attribute__((hot)) void Sema_mark_freed(Sema* self, TrStr name);
__attribute__((hot)) void Sema_check_not_moved(Sema* self, TrStr name, TrStr ty_name);
__attribute__((hot)) void Sema_mark_borrow(Sema* self, TrStr name);
__attribute__((hot)) void Sema_unmark_borrow(Sema* self, TrStr name);
__attribute__((hot)) void Sema_check_no_active_borrows(Sema* self, TrStr name, TrStr ty_name);
__attribute__((hot)) void Sema_mark_init(Sema* self, TrStr name);
__attribute__((hot)) void Sema_clear_container_borrow(Sema* self, TrStr var_name);
__attribute__((hot)) void Sema_unmark_moved(Sema* self, TrStr name);
__attribute__((hot)) void Sema_mark_maybe_moved(Sema* self, TrStr name);
__attribute__((hot)) void Sema_unmark_init(Sema* self, TrStr name);
__attribute__((hot)) void Sema_mark_maybe_init(Sema* self, TrStr name);
__attribute__((hot)) bool Sema_vec_str_contains(Sema* self, List_TrStr* v, TrStr s);
__attribute__((hot)) bool Sema_has_copy_decorator(Sema* self, List_ptr* decs);
__attribute__((hot)) bool Sema_is_copy_class(Sema* self, TrStr name);
__attribute__((hot)) void Sema_enter_scope(Sema* self);
__attribute__((hot)) void Sema_exit_scope(Sema* self);
__attribute__((hot)) bool Sema__coll_elem_droppable(Sema* self, TrStr n);
__attribute__((hot)) void Sema__scan_ptr_aliased_ty(Sema* self, AstType* t);
__attribute__((hot)) bool Sema__is_rc_class(Sema* self, TrStr n);
__attribute__((hot)) bool Sema__ty_reaches_plain_rc(Sema* self, AstType* ft, long long depth);
__attribute__((hot)) bool Sema__sendable_reaches_plain_rc(Sema* self, TrStr tn, long long depth);
__attribute__((hot)) bool Sema__is_unsafe_sendable(Sema* self, TrStr n);
__attribute__((hot)) void Sema__check_spawn_nested_rc(Sema* self, HirExpr* arg_expr);
__attribute__((hot)) void Sema__collect_strong_edge(Sema* self, AstType* ft, List_TrStr* out);
__attribute__((hot)) List_TrStr* Sema__strong_owned(Sema* self, ClassDef* cd);
__attribute__((hot)) void Sema__cycle_dfs(Sema* self, TrStr cur, TrStr start, TrMap* on_path, TrMap* reported, long long depth);
__attribute__((hot)) void Sema_check_ownership_cycles(Sema* self, Program* prog);
__attribute__((hot)) void Sema__scan_class_ptrs(Sema* self, ClassDef* c);
__attribute__((hot)) void Sema__scan_fn_ptrs(Sema* self, FunctionDef* f);
__attribute__((hot)) bool Sema_is_droppable_sym(Sema* self, Symbol* sym);
__attribute__((hot)) void Sema_open_block(Sema* self);
__attribute__((hot)) void Sema_close_block(Sema* self);
__attribute__((hot)) bool Sema_block_stack_contains(Sema* self, long long id);
__attribute__((hot)) List_ptr* Sema_compute_scope_drops(Sema* self, long long scope_idx, bool is_innermost);
__attribute__((hot)) void Sema_append_drops_from(Sema* self, HirBlock* hb, long long from_idx);
__attribute__((hot)) void Sema_append_drops_from_excl(Sema* self, HirBlock* hb, long long from_idx, TrStr exclude);
__attribute__((hot)) void Sema_append_drops_from_excl_multi(Sema* self, HirBlock* hb, long long from_idx, List_TrStr* excludes);
__attribute__((hot)) void Sema_collect_idents(Sema* self, HirExpr* e, List_TrStr* out);
__attribute__((hot)) bool Sema_is_local_var(Sema* self, TrStr name);
__attribute__((hot)) void Sema_mark_str_escaped(Sema* self, TrStr name);
__attribute__((hot)) void Sema_set_borrows_region(Sema* self, TrStr name, TrStr region);
__attribute__((hot)) TrStr Sema_compute_region(Sema* self, Expr* e);
__attribute__((hot)) bool Sema_region_outlives(Sema* self, TrStr longer, TrStr shorter);
__attribute__((hot)) bool Sema_field_is_borrow(Sema* self, Expr* obj, TrStr field);
__attribute__((hot)) void Sema_mark_str_arg(Sema* self, HirExpr* e);
__attribute__((hot)) void Sema_mark_escaped_str_args(Sema* self, HirExpr* e);
__attribute__((hot)) void Sema_mark_coll_escaped(Sema* self, TrStr name);
__attribute__((hot)) void Sema_mark_coll_arg(Sema* self, HirExpr* e);
__attribute__((hot)) void Sema_mark_escaped_coll_args(Sema* self, HirExpr* e);
__attribute__((hot)) bool Sema_block_ends_in_jump(Sema* self, HirBlock* b);
__attribute__((hot)) void Sema_append_block_local_drops(Sema* self, HirBlock* hb, long long block_id);
__attribute__((hot)) void Sema_finalize_scope_drops(Sema* self, HirBlock* hb);
__attribute__((hot)) bool Sema_block_str_decl(Sema* self, HirBlock* hb, TrStr nm);
__attribute__((hot)) bool Sema_block_has_autodrop(Sema* self, HirBlock* hb, TrStr nm);
__attribute__((hot)) TrStr Sema_block_coll_decl(Sema* self, HirBlock* hb, TrStr nm);
__attribute__((hot)) bool Sema_coll_droppable_by_sema(Sema* self, TrStr nm);
__attribute__((hot)) void Sema_apply_mir_if_drops(Sema* self, HirFunction* hf);
__attribute__((hot)) void Sema_declare(Sema* self, TrStr name, SymbolKind kind, AstType** ty, bool is_mut);
__attribute__((hot)) Symbol* Sema_resolve(Sema* self, TrStr name);
__attribute__((hot)) bool Sema_is_known_name(Sema* self, TrStr name);
__attribute__((hot)) bool Sema_is_type_name(Sema* self, TrStr nm);
__attribute__((hot)) TrStr Sema_type_ref_name(Sema* self, Expr* raw);
__attribute__((hot)) bool Sema_is_global_not_local(Sema* self, TrStr name);
__attribute__((hot)) HirProgram* Sema_analyze(Sema* self, Program* prog);
__attribute__((hot)) void Sema_compute_return_ownership(Sema* self, HirProgram* hp);
__attribute__((hot)) void Sema_compute_param_ownership(Sema* self, HirProgram* hp);
__attribute__((hot)) bool Sema__is_pure_scalar(Sema* self, TrStr tn);
__attribute__((hot)) bool Sema__pc_owns_return(Sema* self, HirExpr* e);
__attribute__((hot)) void Sema_apply_borrow_drops(Sema* self, HirProgram* hp);
__attribute__((hot)) void Sema__borrow_drops_block(Sema* self, HirBlock* b);
__attribute__((hot)) void Sema__borrow_drops_recurse_stmt(Sema* self, HirStmt* sp);
__attribute__((hot)) bool Sema__is_fresh_owned_producer(Sema* self, HirExpr* val, TrStr cn);
__attribute__((hot)) bool Sema__is_owned_droppable_class(Sema* self, TrStr cn);
__attribute__((hot)) bool Sema__block_has_drop_or_reassign(Sema* self, HirBlock* b, TrStr nm);
__attribute__((hot)) void Sema__insert_drop_before_terminator(Sema* self, HirBlock* b, TrStr nm, TrStr cn);
__attribute__((hot)) bool Sema__pc_is_ident(Sema* self, HirExpr* ep, TrStr name);
__attribute__((hot)) TrStr Sema__pc_callee_name(Sema* self, HirExpr* callee);
__attribute__((hot)) bool Sema__pc_stores_method(Sema* self, TrStr m);
__attribute__((hot)) bool Sema__pc_is_store_target(Sema* self, HirExpr* tgt);
__attribute__((hot)) bool Sema__pc_refs(Sema* self, HirExpr* ep, TrStr name);
__attribute__((hot)) bool Sema__pc_anyrefs(Sema* self, List_ptr* args, TrStr name);
__attribute__((hot)) bool Sema__param_consumed_in_block(Sema* self, TrStr pname, HirBlock* b);
__attribute__((hot)) bool Sema__param_consumed_in_stmt(Sema* self, TrStr pname, HirStmt* sp);
__attribute__((hot)) bool Sema__param_consumed_in_expr(Sema* self, TrStr pname, HirExpr* ep);
__attribute__((hot)) bool Sema__fn_ret_is_heap_class(Sema* self, HirFunction* f);
__attribute__((hot)) void Sema__collect_returns(Sema* self, HirBlock* b, List_ptr* out);
__attribute__((hot)) void Sema__collect_returns_stmt(Sema* self, HirStmt* sp, List_ptr* out);
__attribute__((hot)) bool Sema__owned_of(Sema* self, TrStr key);
__attribute__((hot)) bool Sema__ret_yields_owned(Sema* self, HirExpr* e);
__attribute__((hot)) bool Sema__is_type_param_in_scope(Sema* self, TrStr name);
__attribute__((hot)) bool Sema__type_satisfies_bound(Sema* self, TrStr type_name, TrStr iface_name);
__attribute__((hot)) AstType** Sema__resolve_generic_bound_method_ret(Sema* self, TrStr pname, TrStr method);
__attribute__((hot)) void Sema_check_call_bounds(Sema* self, TrStr fname, List_ptr* hargs);
__attribute__((hot)) AstType* Sema__subst_ret_generics(Sema* self, AstType* ty, List_TrStr* generics, List_ptr* concrete);
__attribute__((hot)) void Sema_check_class_bounds(Sema* self, TrStr cls_name, List_ptr* arg_tys);
__attribute__((hot)) void Sema_register_decl(Sema* self, Decl* d);
__attribute__((hot)) HirFunction* Sema_lower_func(Sema* self, FunctionDef* f);
__attribute__((hot)) HirClass* Sema_lower_class(Sema* self, ClassDef* c);
__attribute__((hot)) HirEnum* Sema_lower_enum(Sema* self, EnumDef* e);
__attribute__((hot)) HirInterface* Sema_lower_interface(Sema* self, InterfaceDef* i_def);
__attribute__((hot)) void Sema_apply_escape_marks(Sema* self, HirStmt* _hs);
__attribute__((hot)) HirBlock* Sema_lower_block(Sema* self, Block* b);
__attribute__((hot)) HirStmt* Sema_lower_stmt(Sema* self, Stmt* s_ptr);
__attribute__((hot)) AstType* Sema_variant_field_ty(Sema* self, TrStr type_name, TrStr variant_name, long long field_idx);
__attribute__((hot)) void Sema_declare_pattern_binds(Sema* self, Pattern pat);
__attribute__((hot)) void Sema_declare_pattern_binds_typed(Sema* self, Pattern pat, AstType* subj_ty);
__attribute__((hot)) AstType* Sema_str_method_ret_ty(Sema* self, TrStr method);
__attribute__((hot)) void Sema_collect_block_refs(Sema* self, HirBlock* b, List_TrStr* out);
__attribute__((hot)) void Sema_collect_stmt_refs(Sema* self, HirStmt* s, List_TrStr* out);
__attribute__((hot)) HirExpr* Sema_lower_do_value(Sema* self, Block* do_body);
__attribute__((hot)) AstType* Sema_infer_break_type(Sema* self, HirBlock* hb);
__attribute__((hot)) AstType* Sema_infer_break_type_stmt(Sema* self, HirStmt* s);
__attribute__((hot)) void Sema_check_no_heap(Sema* self, TrStr what, TrStr fix);
__attribute__((hot)) HirExpr* Sema_lower_expr(Sema* self, Expr* e_ptr);
__attribute__((hot)) TrStr Sema_is_reserved_error(Sema* self, TrStr name);
__attribute__((hot)) TrStr Sema_is_reserved_keyword(Sema* self, TrStr name);
__attribute__((hot)) bool Sema_block_returns(Sema* self, Block* b);
__attribute__((hot)) void Sema_pattern_covers(Sema* self, Pattern p, List_TrStr* tnames, List_TrStr* vnames);
__attribute__((hot)) bool Sema_is_primitive(Sema* self, AstType* ty);
__attribute__((hot)) bool Sema__is_known_type_name(Sema* self, TrStr name);
__attribute__((hot)) bool Sema_is_primitive_name(Sema* self, TrStr name);
__attribute__((hot)) Symbol** box_symbol(Symbol* s);
__attribute__((malloc,returns_nonnull,hot)) Formatter* Formatter_init(List_i64* c_lines, List_TrStr* c_texts, List_bool* c_trailing);
__attribute__((hot)) TrStr Formatter_pad(Formatter* self);
__attribute__((hot)) void Formatter_line(Formatter* self, TrStr text);
__attribute__((hot)) void Formatter_flush_standalone_before(Formatter* self, long long line_no);
__attribute__((hot)) TrStr Formatter_trailing_for(Formatter* self, long long line_no);
__attribute__((hot)) void Formatter_flush_remaining(Formatter* self);
__attribute__((hot)) TrStr Formatter_escape_str(Formatter* self, TrStr s);
__attribute__((hot)) TrStr Formatter_escape_char(Formatter* self, long long v);
__attribute__((hot)) TrStr Formatter_escape_str_sq(Formatter* self, TrStr s);
__attribute__((hot)) TrStr Formatter_type_str(Formatter* self, AstType** t);
__attribute__((hot)) bool Formatter_needs_parens(Formatter* self, Expr* e);
__attribute__((hot)) TrStr Formatter_operand_str(Formatter* self, Expr* e);
__attribute__((hot)) TrStr Formatter_args_str(Formatter* self, List_ptr* args);
__attribute__((hot)) TrStr Formatter_expr_str(Formatter* self, Expr* e);
__attribute__((hot)) TrStr Formatter_fstring_str(Formatter* self, List_ptr* parts);
__attribute__((hot)) TrStr Formatter_dict_str(Formatter* self, List_ptr* keys, List_ptr* vals);
__attribute__((hot)) TrStr Formatter_comp_str(Formatter* self, Expr* elem, List_ptr* gens);
__attribute__((hot)) TrStr Formatter_slice_str(Formatter* self, Expr* a, Expr* b, Expr* c);
__attribute__((hot)) TrStr Formatter_closure_str(Formatter* self, List_ptr* params, AstType** ret_ty, Block* body, bool is_async);
__attribute__((hot)) TrStr Formatter_params_str(Formatter* self, List_ptr* params);
__attribute__((hot)) void Formatter_emit_decorators(Formatter* self, List_ptr* decs);
__attribute__((hot)) void Formatter_emit_block(Formatter* self, Block* b);
__attribute__((hot)) void Formatter_emit_simple(Formatter* self, TrStr text, long long src_line);
__attribute__((hot)) void Formatter_emit_stmt(Formatter* self, Stmt* sp, long long src_line);
__attribute__((hot)) void Formatter_emit_inline_stmt(Formatter* self, Stmt* sp);
__attribute__((hot)) TrStr Formatter_let_str(Formatter* self, TrStr name, Ownership own, bool is_mut, bool is_const, bool is_shared, AstType** ty, Expr* val);
__attribute__((hot)) TrStr Formatter_pattern_str(Formatter* self, Pattern p);
__attribute__((hot)) TrStr Formatter_fn_header(Formatter* self, FunctionDef* f);
__attribute__((hot)) void Formatter_emit_function(Formatter* self, FunctionDef* f);
__attribute__((hot)) void Formatter_emit_class(Formatter* self, ClassDef* c, TrStr kw);
__attribute__((hot)) void Formatter_emit_enum(Formatter* self, EnumDef* e);
__attribute__((hot)) void Formatter_emit_interface(Formatter* self, InterfaceDef* it);
__attribute__((hot)) void Formatter_emit_decl(Formatter* self, Decl* dp);
__attribute__((hot)) TrStr Formatter_fn_header_no_colon(Formatter* self, FunctionDef* f);
__attribute__((hot)) bool Formatter_is_block_decl(Formatter* self, Decl* dp);
__attribute__((hot)) TrStr Formatter_format_program(Formatter* self, Program* prog);
__attribute__((malloc,returns_nonnull,hot)) CTok* CTok_init(long long kind, TrStr text);
__attribute__((hot)) bool _is_alpha(long long c);
__attribute__((hot)) bool _is_digit(long long c);
__attribute__((hot)) bool _is_alnum(long long c);
__attribute__((hot)) bool _is_space(long long c);
__attribute__((hot)) List_ptr* tokenize_c(TrStr src);
__attribute__((hot)) TrStr map_base(TrStr words);
__attribute__((hot)) TrStr map_type(TrStr words, long long stars);
__attribute__((hot)) TrMap* _runtime_symbols();
__attribute__((hot)) bool _is_prim_type_word(TrStr w);
__attribute__((hot)) bool _is_ignored_word(TrStr w);
__attribute__((hot)) bool _is_decl_term(long long kind, TrStr text);
__attribute__((hot)) TrStr _join_words(List_TrStr* words);
__attribute__((hot)) long long _to_int(TrStr s);
__attribute__((hot)) long long _skip_gnu_attrs(List_ptr* toks, long long pos);
__attribute__((hot)) TrStr _scan_typedef_name(List_ptr* toks, long long pos);
__attribute__((hot)) bool _typedef_is_fnptr(List_ptr* toks, long long pos);
__attribute__((hot)) TrStr _scan_fnptr_name(List_ptr* toks, long long pos);
__attribute__((malloc,returns_nonnull,hot)) Bindgen* Bindgen_init(List_ptr* toks);
__attribute__((hot)) bool Bindgen_fresh(Bindgen* self, TrStr name);
__attribute__((hot)) TrStr Bindgen_ct(Bindgen* self);
__attribute__((hot)) long long Bindgen_ck(Bindgen* self);
__attribute__((hot)) void Bindgen_adv(Bindgen* self);
__attribute__((hot)) bool Bindgen_is_punct(Bindgen* self, TrStr p);
__attribute__((hot)) long long Bindgen_nk(Bindgen* self);
__attribute__((hot)) TrStr Bindgen_nt(Bindgen* self);
__attribute__((hot)) void Bindgen_skip_balanced_parens(Bindgen* self);
__attribute__((hot)) void Bindgen_skip_struct_body(Bindgen* self);
__attribute__((hot)) void Bindgen_skip_to_semi(Bindgen* self);
__attribute__((hot)) void Bindgen_skip_braces(Bindgen* self);
__attribute__((hot)) void Bindgen_skip_attributes(Bindgen* self);
__attribute__((hot)) bool Bindgen_func_def_follows(Bindgen* self);
__attribute__((hot)) List_TrStr* Bindgen_read_type_words(Bindgen* self);
__attribute__((hot)) void Bindgen_emit_func(Bindgen* self, TrStr ret_words, long long ret_stars, TrStr name);
__attribute__((hot)) void Bindgen_emit_struct(Bindgen* self, TrStr name, bool is_union);
__attribute__((hot)) void Bindgen_skip_enum_expr(Bindgen* self);
__attribute__((hot)) void Bindgen_emit_enum(Bindgen* self, TrStr name);
__attribute__((hot)) void Bindgen_parse_decl(Bindgen* self);
__attribute__((hot)) void Bindgen_run(Bindgen* self);
__attribute__((hot)) bool _is_ident_byte(long long c);
__attribute__((hot)) TrStr _rename_word(TrStr text, TrStr old, TrStr new_);
__attribute__((hot)) TrStr _basename(TrStr p);
__attribute__((hot)) TrStr _marker_file(TrStr line);
__attribute__((hot)) bool _marker_is_system(TrStr line);
__attribute__((hot)) TrStr _filter_to_target(TrStr raw, TrStr target);
__attribute__((hot)) TrStr _macro_name(TrStr rest);
__attribute__((hot)) TrMap* _load_baseline(TrStr cc);
__attribute__((hot)) TrMap* _target_define_names(TrStr header);
__attribute__((hot)) TrStr _lstrip(TrStr s);
__attribute__((hot)) bool _is_builtin_ty_name(TrStr n);
__attribute__((hot)) bool _is_libc_ty_name(TrStr n);
__attribute__((hot)) bool _is_system_record_ty(TrStr n);
__attribute__((hot)) TrStr _ident_at(TrStr text, long long start);
__attribute__((hot)) TrStr _opaque_fallbacks(TrStr body, TrMap* defined);
__attribute__((hot)) bool _is_single_string_literal(TrStr val);
__attribute__((hot)) bool _is_single_int_literal(TrStr val);
__attribute__((hot)) void emit_defines(Bindgen* bg, TrStr defs, TrMap* baseline, TrMap* allow);
__attribute__((hot)) TrStr _cxx_of(TrStr cc);
__attribute__((hot)) TrStr _macro_args(long long n, long long mode, long long atype, long long tp);
__attribute__((hot)) TrStr _ent_name(TrStr e);
__attribute__((hot)) long long _ent_arity(TrStr e);
__attribute__((hot)) long long _ent_typepos(TrStr e);
__attribute__((hot)) bool _is_ident_ch(long long c);
__attribute__((hot)) bool _param_ptr_type(TrStr body, TrStr pname);
__attribute__((hot)) bool _balanced_delims(TrStr s);
__attribute__((hot)) List_TrStr* _collect_fn_macros(TrStr defs, TrMap* baseline);
__attribute__((hot)) bool _has_cc_error(TrStr errtxt);
__attribute__((hot)) TrMap* _macro_bad_names(TrStr errtxt);
__attribute__((hot)) TrStr _macro_shim_line(TrStr sym, TrStr nm, long long ar, long long form, long long atype, long long tp);
__attribute__((hot)) List_TrStr* _macro_probe_write(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake);
__attribute__((hot)) TrMap* _macro_form_bad(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake, TrStr cxx, TrStr extra);
__attribute__((hot)) TrMap* _macro_verify_form(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake, TrStr cxx, TrStr extra);
__attribute__((hot)) TrMap* _macro_remaining(List_TrStr* macros, TrMap* done);
__attribute__((hot)) void _macro_mark_done(TrMap* done, TrMap* ok, List_TrStr* macros);
__attribute__((hot)) void _macro_pass(TrStr header, List_TrStr* macros, TrMap* done, TrMap* plan, long long form, long long atype, bool bake, TrStr cxx, TrStr extra);
__attribute__((hot)) TrStr _gen_macro_shims(TrStr header, TrStr out, List_TrStr* macros, TrStr cxx, TrStr extra);
__attribute__((hot)) void run_bindgen(TrStr header, TrStr out, TrStr cc, TrStr extra, TrStr pkglibs, bool want_macros);
__attribute__((hot)) TrStr _cxxwalk_src();
__attribute__((hot)) void _rm_files(TrStr files);
__attribute__((hot)) TrStr _local_exe(TrStr stem);
__attribute__((hot)) TrStr _detect_libclang(TrStr cc);
__attribute__((hot)) CppType* _cpp_parse_type(TrStr spelling);
__attribute__((hot)) TrStr _last_seg(TrStr s);
__attribute__((hot)) TrStr _cpp_op_name(TrStr mname, long long nparams, bool is_member);
__attribute__((hot)) TrStr _cpp_ident(TrStr s);
__attribute__((hot)) bool _is_clean_ident(TrStr s);
__attribute__((hot)) bool _is_tr_keyword(TrStr n);
__attribute__((hot)) TrStr _uniq_sym(TrStr base_sym, TrMap* used);
__attribute__((hot)) TrStr _cpp_tr_pname(TrStr pname);
__attribute__((hot)) TrStr _cpp_ctype(CppType* t);
__attribute__((hot)) TrStr _cpp_tr_type(CppType* t);
__attribute__((hot)) List_TrStr* _cpp_ret(CppType* rt, TrStr call);
__attribute__((hot)) TrStr _cpp_opaque_handle(TrStr base, long long nd, TrMap* class_names, TrMap* value_structs, TrMap* seen, StringBuilder* opaque);
__attribute__((hot)) TrStr _cpp_qual(TrStr base, TrMap* class_qual);
__attribute__((hot)) List_TrStr* _cpp_ret_ex(TrStr desc, TrStr call, TrMap* value_structs, TrMap* class_names, TrMap* class_qual, TrMap* seen, StringBuilder* opaque);
__attribute__((hot)) TrStr _ns_pop(TrStr path);
__attribute__((hot)) TrStr _ns_us(TrStr path);
__attribute__((hot)) TrStr _rstrip_cr(TrStr s);
__attribute__((hot)) TrStr _c_to_cpp(TrStr cn);
__attribute__((hot)) List_TrStr* _desc4(TrStr desc);
__attribute__((hot)) List_TrStr* _parse_tclass(TrStr rest);
__attribute__((hot)) TrStr _stars(long long n);
__attribute__((hot)) TrStr _ptr_wrap(TrStr inner, long long n);
__attribute__((hot)) TrStr _cpp_field_type(TrStr desc, TrMap* value_structs, TrMap* enum_names);
__attribute__((hot)) TrStr _shim_body(TrStr ret_ctype, TrStr body);
__attribute__((hot)) TrStr _fnptr_cast(TrStr fnty);
__attribute__((hot)) void _cpp_generate(TrStr ir, TrStr header, TrStr out, TrStr shim_cflags, TrStr pkglibs);
__attribute__((hot)) TrStr _cpp_detect_include_dirs(TrStr cc);
__attribute__((hot)) TrStr _cpp_std_flag(TrStr extra);
__attribute__((hot)) long long _cpp_fatal_count(TrStr diag);
__attribute__((hot)) void _cpp_print_diag(TrStr diag);
__attribute__((hot)) bool _cpp_ir_is_empty(TrStr ir);
__attribute__((hot)) void _cpp_cleanup();
__attribute__((hot)) bool _is_expr_proxy_spec(TrStr s);
__attribute__((hot)) List_TrStr* _collect_specs(TrStr ir);
__attribute__((hot)) void run_bindgen_cpp(TrStr header, TrStr out, TrStr cc, TrStr extra, TrStr pkglibs);
__attribute__((hot)) TrStr _c_dot_to_safe(TrStr s);
__attribute__((hot)) TrStr _indent_str(long long n);
__attribute__((hot)) bool _is_invalid_ptr(unsigned long long addr);
__attribute__((hot)) bool _is_str_type(TrStr n);
__attribute__((hot)) bool _is_int_type(TrStr n);
__attribute__((hot)) bool _is_float_type(TrStr n);
__attribute__((hot)) bool _is_c_int_scalar(TrStr n);
__attribute__((hot)) TrStr _safe_c_varname(TrStr n);
__attribute__((hot)) bool _is_c_keyword(TrStr n);
__attribute__((hot)) bool _starts_with_tr(TrStr s);
__attribute__((hot)) bool _is_primitive(TrStr n);
__attribute__((hot)) TrStr _escape_str_for_c(TrStr s);
__attribute__((hot)) TrStr _escape_fmt_for_c(TrStr s);
__attribute__((hot)) LInst* box_linst(LInst i);
__attribute__((malloc,returns_nonnull,hot)) LBlock* LBlock_init(long long id);
__attribute__((malloc,returns_nonnull,hot)) LFunc* LFunc_init(TrStr name);
__attribute__((hot)) long long LFunc_fresh_id(LFunc* self);
__attribute__((hot)) long long LFunc_new_block(LFunc* self);
__attribute__((hot)) void LFunc_set_cur(LFunc* self, long long id);
__attribute__((hot)) void LFunc_emit(LFunc* self, LInst i);
__attribute__((hot)) void LFunc_set_term(LFunc* self, LTerm t);
__attribute__((hot)) bool LFunc_cur_terminated(LFunc* self);
__attribute__((hot)) long long LFunc_new_vreg(LFunc* self);
__attribute__((hot)) void LFunc_set_vreg_type(LFunc* self, long long id, long long t);
__attribute__((hot)) long long LFunc_vreg_type(LFunc* self, long long id);
__attribute__((hot)) void LFunc_add_var(LFunc* self, TrStr name);
__attribute__((hot)) long long LFunc_var_index(LFunc* self, TrStr name);
__attribute__((hot)) void LFunc_set_var_type(LFunc* self, TrStr name, long long t);
__attribute__((hot)) long long LFunc_var_type(LFunc* self, TrStr name);
__attribute__((hot)) void LFunc_set_var_cls(LFunc* self, TrStr name, TrStr cls);
__attribute__((hot)) TrStr LFunc_var_cls_of(LFunc* self, TrStr name);
__attribute__((hot)) void LFunc_set_var_arr(LFunc* self, TrStr name, long long n);
__attribute__((hot)) long long LFunc_var_arr_of(LFunc* self, TrStr name);
__attribute__((hot)) void LFunc_set_vreg_xret(LFunc* self, long long id, long long t);
__attribute__((hot)) long long LFunc_vreg_xret_of(LFunc* self, long long id);
__attribute__((hot)) void LFunc_set_var_xret(LFunc* self, TrStr name, long long t);
__attribute__((hot)) long long LFunc_var_xret_of(LFunc* self, TrStr name);
__attribute__((hot)) long long LFunc_capture_index(LFunc* self, TrStr name);
__attribute__((malloc,returns_nonnull,hot)) ClassLayout* ClassLayout_init(TrStr name);
__attribute__((hot)) long long ClassLayout_field_index(ClassLayout* self, TrStr fname);
__attribute__((malloc,returns_nonnull,hot)) VariantLayout* VariantLayout_init(TrStr name);
__attribute__((malloc,returns_nonnull,hot)) EnumLayout* EnumLayout_init(TrStr name);
__attribute__((hot)) long long EnumLayout_variant_index(EnumLayout* self, TrStr vname);
__attribute__((malloc,returns_nonnull,hot)) LModule* LModule_init();
__attribute__((hot)) void LModule_mark_overloaded(LModule* self, TrStr cls, TrStr method);
__attribute__((hot)) bool LModule_is_overloaded(LModule* self, TrStr cls, TrStr method);
__attribute__((hot)) TrStr LModule_resolve_method_ov(LModule* self, TrStr cls, TrStr method, long long argc);
__attribute__((hot)) long long LModule_add_global(LModule* self, TrStr name, long long tag);
__attribute__((hot)) void LModule_set_global_arr(LModule* self, long long idx, long long n);
__attribute__((hot)) long long LModule_global_arr_of(LModule* self, TrStr name);
__attribute__((hot)) long long LModule_global_index(LModule* self, TrStr name);
__attribute__((hot)) bool LModule_is_global(LModule* self, TrStr name);
__attribute__((hot)) long long LModule_global_type(LModule* self, TrStr name);
__attribute__((hot)) long long LModule_fn_ret_tag(LModule* self, TrStr name);
__attribute__((hot)) long long LModule_add_string(LModule* self, TrStr s);
__attribute__((hot)) void LModule_add_extern(LModule* self, TrStr name);
__attribute__((hot)) bool LModule_is_user_fn(LModule* self, TrStr name);
__attribute__((hot)) bool LModule_is_extern_fn(LModule* self, TrStr name);
__attribute__((hot)) long long LModule_extern_ret_tag(LModule* self, TrStr name);
__attribute__((hot)) void LModule_mark_fn_owned(LModule* self, TrStr name);
__attribute__((hot)) bool LModule_fn_ret_owned(LModule* self, TrStr name);
__attribute__((hot)) long long LModule_unavail_index(LModule* self, TrStr name);
__attribute__((hot)) void LModule_add_class(LModule* self, ClassLayout* cl);
__attribute__((hot)) long long LModule_class_index(LModule* self, TrStr name);
__attribute__((hot)) bool LModule_is_class(LModule* self, TrStr name);
__attribute__((hot)) bool LModule_is_value_class(LModule* self, TrStr name);
__attribute__((hot)) long long LModule_class_size(LModule* self, TrStr name);
__attribute__((hot)) long long LModule_field_offset(LModule* self, TrStr cls, TrStr fld);
__attribute__((hot)) long long LModule_field_tag(LModule* self, TrStr cls, TrStr fld);
__attribute__((hot)) TrStr LModule_resolve_method(LModule* self, TrStr cls, TrStr method);
__attribute__((hot)) TrStr LModule_field_cls(LModule* self, TrStr cls, TrStr fld);
__attribute__((hot)) void LModule_add_enum(LModule* self, EnumLayout* el);
__attribute__((hot)) long long LModule_enum_index(LModule* self, TrStr name);
__attribute__((hot)) bool LModule_is_enum(LModule* self, TrStr name);
__attribute__((hot)) long long LModule_enum_size(LModule* self, TrStr name);
__attribute__((hot)) long long LModule_enum_variant_index(LModule* self, TrStr ename, TrStr vname);
__attribute__((hot)) void LModule_add_iface(LModule* self, TrStr name);
__attribute__((hot)) bool LModule_is_iface(LModule* self, TrStr name);
__attribute__((hot)) AstType** box_asttype_lir(AstType* t);
__attribute__((hot)) long long _f64_bits(double v);
__attribute__((hot)) long long _promote_f(LFunc* lf, long long v);
__attribute__((hot)) long long _int_cast_width(TrStr tn);
__attribute__((hot)) bool _int_cast_signed(TrStr tn);
__attribute__((hot)) long long _narrow_int(LFunc* lf, long long v, TrStr tn);
__attribute__((hot)) bool _is_int_cast_target(TrStr tn);
__attribute__((hot)) TrStr _print_i64_sym();
__attribute__((hot)) bool _is_list_tag(long long t);
__attribute__((hot)) long long _list_stride(long long ltag);
__attribute__((hot)) bool _is_set_tag(long long t);
__attribute__((hot)) TrStr _set_sym(long long t, TrStr op);
__attribute__((hot)) bool _is_dict_tag(long long t);
__attribute__((hot)) bool _dict_key_is_str(long long t);
__attribute__((hot)) long long _dict_val_tag(long long t);
__attribute__((hot)) TrStr _dict_new_sym(long long t);
__attribute__((hot)) TrStr _dict_sym(long long t, TrStr op);
__attribute__((hot)) long long _list_elem_tag(long long t);
__attribute__((hot)) long long _list_tag_for_elem(long long et);
__attribute__((hot)) long long _list_tag_from_ann(LModule* m, AstType* ty);
__attribute__((hot)) long long _dict_tag_from_ann(LModule* m, AstType* ty);
__attribute__((hot)) bool _is_cmp_op(TrStr op);
__attribute__((hot)) bool _is_int_typename(TrStr n);
__attribute__((hot)) long long _ast_type_tag(AstType* ty);
__attribute__((hot)) bool _is_null_str(TrStr s);
__attribute__((hot)) TrStr _own(TrStr s);
__attribute__((hot)) long long _tag_of(LModule* m, AstType* ty);
__attribute__((hot)) AstType* _subst_args(LModule* m, AstType* ty);
__attribute__((hot)) AstType* _subst_ty(LModule* m, AstType* ty);
__attribute__((hot)) long long _prog_generic_class_index(LModule* m, TrStr name);
__attribute__((hot)) TrStr _mangle_generic(LModule* m, AstType* ty);
__attribute__((hot)) TrStr _ensure_generic_class(LModule* m, AstType* ty);
__attribute__((hot)) TrStr _val_obj_cls(LModule* m, LFunc* lf, HirExpr* val);
__attribute__((hot)) TrStr _cls_of_ty(LModule* m, AstType* ty);
__attribute__((hot)) TrStr _recv_class(LModule* m, LFunc* lf, HirExpr* obj);
__attribute__((hot)) long long _lower_list_comp(LModule* m, LFunc* lf, HirExpr* element, List_ptr* generators, AstType* lty);
__attribute__((hot)) AstType* _hir_method_ret_ty(LModule* m, TrStr cls, TrStr method);
__attribute__((hot)) long long _prog_class_index(HirProgram* prog, TrStr name);
__attribute__((hot)) bool _push_field_names_rec(HirProgram* prog, long long ci, ClassLayout* lay, long long depth);
__attribute__((hot)) bool _push_field_tags_rec(LModule* m, HirProgram* prog, long long ci, ClassLayout* lay, long long depth);
__attribute__((hot)) void _register_classes(LModule* m, HirProgram* prog);
__attribute__((hot)) bool _class_needs_drop(ClassLayout* lay);
__attribute__((hot)) bool _class_needs_drop_by_name(LModule* m, TrStr cname);
__attribute__((hot)) void _attach_class_drop(LModule* m, LFunc* lf, long long obj, TrStr cname);
__attribute__((hot)) void _build_class_drop(LModule* m, ClassLayout* lay);
__attribute__((hot)) void _gen_class_drops(LModule* m);
__attribute__((hot)) bool _method_in_prog_functions(HirProgram* prog, TrStr cls, TrStr name);
__attribute__((hot)) long long _method_nonself_pcount(HirFunction* f);
__attribute__((hot)) TrStr _ov_method_name(LModule* m, TrStr cls, HirFunction* f);
__attribute__((hot)) void _detect_overloads(LModule* m, HirProgram* prog);
__attribute__((hot)) long long _arg_limit(LModule* m);
__attribute__((hot)) LModule* lower_to_lir_t(HirProgram* prog, bool llvm);
__attribute__((hot)) LModule* lower_to_lir_nh(HirProgram* prog, bool llvm, bool no_heap);
__attribute__((hot)) LModule* lower_to_lir(HirProgram* prog);
__attribute__((hot)) bool _is_extern_c_int(TrStr n);
__attribute__((hot)) bool _is_extern_c_float(TrStr n);
__attribute__((hot)) long long _extern_sig_tag(LModule* m, AstType* ty);
__attribute__((hot)) LModule* _lower_to_lir_body(LModule* m, HirProgram* prog);
__attribute__((hot)) bool _fn_has_iface_param(LModule* m, HirFunction* f);
__attribute__((hot)) bool _fn_is_specializable(LModule* m, HirFunction* f);
__attribute__((hot)) long long _find_generic_fn(LModule* m, TrStr name);
__attribute__((hot)) long long _find_generic_method(LModule* m, TrStr cls, TrStr method);
__attribute__((hot)) bool _is_generic_param(HirFunction* f, TrStr n);
__attribute__((hot)) bool _param_is_abstract(LModule* m, HirFunction* f, TrStr ptyname);
__attribute__((hot)) bool _lir_lower_generic(LModule* m, HirFunction* f, List_i64* argtags, List_TrStr* argcls, TrStr mangled);
__attribute__((hot)) TrStr _mono_base(TrStr name);
__attribute__((hot)) TrStr _mono_concrete(TrStr name);
__attribute__((hot)) bool _lir_lower_mono_fn(LModule* m, HirFunction* f, TrStr mangled, AstType* concrete);
__attribute__((hot)) void _lir_lower_method(LModule* m, TrStr class_name, HirFunction* f);
__attribute__((hot)) bool _register_global(LModule* m, HirStmt* s);
__attribute__((hot)) bool _lower_global_init(LModule* m, LFunc* lf, HirStmt* s);
__attribute__((hot)) void _lir_lower_function(LModule* m, HirFunction* f);
__attribute__((hot)) bool _field_tag_ok(long long vt, long long ftg);
__attribute__((hot)) void _emit_field_set(LModule* m, LFunc* lf, long long obj, long long off, long long val);
__attribute__((hot)) long long _emit_field_get(LModule* m, LFunc* lf, long long obj, long long off, long long tag);
__attribute__((hot)) long long _lower_enum_ctor(LModule* m, LFunc* lf, TrStr ename, TrStr vname, List_ptr* margs);
__attribute__((hot)) long long _lir_fn_ret_tag(LModule* m, HirFunction* f);
__attribute__((hot)) long long _wrap_result(LModule* m, LFunc* lf, long long vidx, long long payv, long long paytag);
__attribute__((hot)) long long _find_free_fn_idx(LModule* m, TrStr name);
__attribute__((hot)) bool _ensure_await_wrapper(LModule* m, TrStr fn);
__attribute__((hot)) long long _lower_await(LModule* m, LFunc* lf, HirExpr* awexpr);
__attribute__((hot)) bool _lower_spawn(LModule* m, LFunc* lf, HirExpr* expr);
__attribute__((hot)) long long _box_call(LModule* m, LFunc* lf, long long selfv, TrStr rtfn, List_ptr* margs, long long nargs, long long rtag);
__attribute__((hot)) long long _pack_spawn_args(LModule* m, LFunc* lf, List_ptr* margs, long long start);
__attribute__((hot)) long long _lower_pool_spawn(LModule* m, LFunc* lf, long long poolv, List_ptr* margs);
__attribute__((hot)) bool _is_runtime_fn(TrStr fn);
__attribute__((hot)) bool _is_rc_str_runtime_fn(TrStr fn);
__attribute__((hot)) void _track_mutex_unlock(LFunc* lf, HirExpr* obj);
__attribute__((hot)) long long _atomic_delta(LModule* m, LFunc* lf, long long selfv, TrStr rtfn);
__attribute__((hot)) long long _lower_box_method(LModule* m, LFunc* lf, HirExpr* obj, TrStr method, List_ptr* margs);
__attribute__((hot)) bool _lower_taskgroup(LModule* m, LFunc* lf, HirBlock* body);
__attribute__((hot)) long long _lower_obj_call(LModule* m, LFunc* lf, TrStr mangled, long long self_vreg, List_ptr* margs);
__attribute__((hot)) bool lower_block(LModule* m, LFunc* lf, HirBlock* hb);
__attribute__((hot)) bool _run_defers(LModule* m, LFunc* lf);
__attribute__((hot)) long long _sizeof_tyname(TrStr n);
__attribute__((hot)) long long _ptr_stride(LModule* m, AstType* pty);
__attribute__((hot)) AstType* _class_field_ty(LModule* m, TrStr cls, TrStr fld);
__attribute__((hot)) AstType* _ptr_chain_ty(LModule* m, HirExpr* e);
__attribute__((hot)) TrStr _dunder_for_op(TrStr op);
__attribute__((hot)) TrStr _stmt_expr_kind(HirExpr* e);
__attribute__((hot)) TrStr _expr_kind(HirExpr* e);
__attribute__((hot)) TrStr _stmt_kind(HirStmt* s);
__attribute__((hot)) bool _stmt_has_cf(HirStmt* s);
__attribute__((hot)) bool _block_has_cf(HirBlock* b);
__attribute__((hot)) bool _lower_try_setjmp(LModule* m, LFunc* lf, HirBlock* try_body, List_ptr* catches, HirBlock* finally_b);
__attribute__((hot)) bool _is_vstruct_lvalue(HirExpr* e);
__attribute__((hot)) long long _copy_value_struct(LModule* m, LFunc* lf, long long src, TrStr cls);
__attribute__((hot)) long long _value_struct_generic_size(LModule* m, AstType* ty);
__attribute__((hot)) bool _lower_stmt_impl(LModule* m, LFunc* lf, HirStmt* s);
__attribute__((hot)) bool lower_stmt(LModule* m, LFunc* lf, HirStmt* s);
__attribute__((hot)) long long _lower_set_method(LModule* m, LFunc* lf, long long shv, long long stag, TrStr method, List_ptr* margs);
__attribute__((hot)) long long _lit_pat_cond(LModule* m, LFunc* lf, Pattern pat, long long subj, long long st);
__attribute__((hot)) bool _lower_match(LModule* m, LFunc* lf, HirExpr* expr, List_ptr* arms);
__attribute__((hot)) TrStr _norm_variant(TrStr ename, TrStr vn);
__attribute__((hot)) long long _variant_tag_cond(LFunc* lf, long long tagv, long long vidx);
__attribute__((hot)) long long _load_enum_payload_field(LModule* m, LFunc* lf, long long subj, AstType* subj_ty, VariantLayout* vlay, long long fldidx);
__attribute__((hot)) long long _lower_enum_prop(LModule* m, LFunc* lf, HirExpr* obj, TrStr ename, TrStr prop);
__attribute__((hot)) bool _bind_payload(LModule* m, LFunc* lf, VariantLayout* vlay, long long subj, AstType* subj_ty, long long fldidx, TrStr bindname);
__attribute__((hot)) bool _lower_match_enum(LModule* m, LFunc* lf, HirExpr* expr, long long subj, List_ptr* arms);
__attribute__((hot)) bool _lower_for(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body);
__attribute__((hot)) bool _sel_recv_arm(LModule* m, LFunc* lf, TrStr donev, HirChanSelectArm* arm);
__attribute__((hot)) bool _sel_send_arm(LModule* m, LFunc* lf, TrStr donev, HirChanSelectArm* arm);
__attribute__((hot)) bool _sel_uncond_arm(LModule* m, LFunc* lf, TrStr donev, HirBlock* body);
__attribute__((hot)) bool _sel_timeout_arm(LModule* m, LFunc* lf, TrStr donev, TrStr tstart, HirChanSelectArm* arm);
__attribute__((hot)) bool _lower_chan_select(LModule* m, LFunc* lf, List_ptr* cases);
__attribute__((hot)) bool _lower_for_chan(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body);
__attribute__((hot)) bool _lower_for_iterproto(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body);
__attribute__((hot)) bool _lower_for_erange(LModule* m, LFunc* lf, TrStr var, HirExpr* start, HirExpr* end, bool inclusive, HirBlock* body);
__attribute__((hot)) bool _lower_for_range(LModule* m, LFunc* lf, TrStr var, List_ptr* args, HirBlock* body);
__attribute__((hot)) bool _lower_for_list(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body);
__attribute__((hot)) bool _lower_for_unpack(LModule* m, LFunc* lf, List_TrStr* vars, HirExpr* iter, HirBlock* body);
__attribute__((hot)) bool _lower_enumerate(LModule* m, LFunc* lf, TrStr ivar, TrStr evar, HirExpr* listexpr, HirBlock* body);
__attribute__((hot)) bool _lower_zip(LModule* m, LFunc* lf, TrStr v0, TrStr v1, HirExpr* aexpr, HirExpr* bexpr, HirBlock* body);
__attribute__((hot)) bool _lower_dict_items_unpack(LModule* m, LFunc* lf, TrStr kvar, TrStr vvar, HirExpr* dictexpr, HirBlock* body);
__attribute__((hot)) void _emit_incr(LFunc* lf, TrStr name);
__attribute__((hot)) TrStr _ident_name(HirExpr* e);
__attribute__((hot)) bool _lower_field_set(LModule* m, LFunc* lf, HirExpr* obj, TrStr prop, HirExpr* val);
__attribute__((hot)) bool _lower_index_set(LModule* m, LFunc* lf, HirExpr* obj, HirExpr* idx, HirExpr* val);
__attribute__((hot)) TrStr _write_sym(long long t);
__attribute__((hot)) void _emit_call0(LModule* m, LFunc* lf, TrStr sym);
__attribute__((hot)) bool _lower_print(LModule* m, LFunc* lf, List_ptr* args);
__attribute__((hot)) bool _lower_assert_cmp(LModule* m, LFunc* lf, TrStr fname, List_ptr* args);
__attribute__((hot)) bool lower_expr_stmt(LModule* m, LFunc* lf, HirExpr* e);
__attribute__((hot)) bool _int_op(TrStr op);
__attribute__((hot)) TrStr _lir_digit(long long d);
__attribute__((hot)) TrStr _lir_itoa(long long n);
__attribute__((hot)) void _fresh_mark(LFunc* lf, long long v);
__attribute__((hot)) bool _fresh_take(LFunc* lf, long long v);
__attribute__((hot)) void _release_str(LModule* m, LFunc* lf, long long v);
__attribute__((hot)) void _retain_str(LModule* m, LFunc* lf, long long v);
__attribute__((hot)) void _flush_fresh_strs(LModule* m, LFunc* lf);
__attribute__((hot)) void _secure_str(LModule* m, LFunc* lf, long long v);
__attribute__((hot)) void _fresh_mark_obj(LFunc* lf, long long v);
__attribute__((hot)) bool _fresh_take_obj(LFunc* lf, long long v);
__attribute__((hot)) void _release_obj(LModule* m, LFunc* lf, long long v);
__attribute__((hot)) void _retain_obj(LModule* m, LFunc* lf, long long v);
__attribute__((hot)) void _flush_fresh_objs(LModule* m, LFunc* lf);
__attribute__((hot)) bool _is_owned_local_return(LFunc* lf, HirExpr* val);
__attribute__((hot)) void _secure_obj(LModule* m, LFunc* lf, long long v);
__attribute__((hot)) bool _is_param(LFunc* lf, TrStr name);
__attribute__((hot)) long long _norm_bool(LFunc* lf, long long v);
__attribute__((hot)) long long _str_call0(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long restype);
__attribute__((hot)) long long _heap_lit(LModule* m, LFunc* lf, TrStr s);
__attribute__((hot)) long long _obj_to_str(LModule* m, LFunc* lf, HirExpr* objexpr, long long objreg);
__attribute__((hot)) long long _obj_repr(LModule* m, LFunc* lf, HirExpr* objexpr, long long objreg);
__attribute__((hot)) long long _reg_to_str(LModule* m, LFunc* lf, long long reg);
__attribute__((hot)) long long _str_concat2(LModule* m, LFunc* lf, long long a, long long b);
__attribute__((hot)) long long _tuple_to_str(LModule* m, LFunc* lf, long long tup, AstType* ty);
__attribute__((hot)) long long _str_call1(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long arg, long long restype);
__attribute__((hot)) long long _lower_str_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs);
__attribute__((hot)) TrStr _float_unary_sym(TrStr method);
__attribute__((hot)) long long _lower_int_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs);
__attribute__((hot)) long long _lower_dict_method(LModule* m, LFunc* lf, long long _tr_v_recv, long long dtag, TrStr method, List_ptr* margs);
__attribute__((hot)) long long _lower_float_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs);
__attribute__((hot)) bool _is_const_int(HirExpr* e);
__attribute__((hot)) long long _const_int_val(HirExpr* e);
__attribute__((hot)) void _emit_add_const(LFunc* lf, TrStr name, long long delta);
__attribute__((hot)) long long _list_call1(LModule* m, LFunc* lf, TrStr sym, long long handle, long long restype);
__attribute__((hot)) long long _inline_list_addr(LModule* m, LFunc* lf, long long handle, long long idx, long long stride);
__attribute__((hot)) long long _fixed_arr_len(LModule* m, LFunc* lf, TrStr name);
__attribute__((hot)) long long _fixed_arr_elem_tag(LModule* m, LFunc* lf, TrStr name);
__attribute__((hot)) long long _array_elem_addr(LModule* m, LFunc* lf, TrStr name, long long idx);
__attribute__((hot)) long long _list_get(LModule* m, LFunc* lf, long long handle, long long idx);
__attribute__((hot)) long long _list_get_raw(LModule* m, LFunc* lf, long long ltag, long long handle, long long idx);
__attribute__((hot)) long long _list_get_elem(LModule* m, LFunc* lf, long long ltag, long long handle, long long idx);
__attribute__((hot)) long long _lower_expr_impl(LModule* m, LFunc* lf, HirExpr* e);
__attribute__((hot)) long long lower_expr(LModule* m, LFunc* lf, HirExpr* e);
__attribute__((hot)) TrStr _ll_ty(long long tag);
__attribute__((hot)) TrStr _ll_ty_name(TrStr n);
__attribute__((hot)) long long _ll_hexdigit(long long n);
__attribute__((hot)) TrStr _ll_hexpad16(long long v);
__attribute__((hot)) long long _ll_str_bytelen(TrStr s);
__attribute__((hot)) TrStr _ll_str_escape(TrStr s);
__attribute__((malloc,returns_nonnull,hot)) LlvmEmitter* LlvmEmitter_init(LModule* m);
__attribute__((hot)) void LlvmEmitter_w(LlvmEmitter* self, TrStr s);
__attribute__((hot)) TrStr LlvmEmitter_newtmp(LlvmEmitter* self);
__attribute__((hot)) TrStr LlvmEmitter_vty(LlvmEmitter* self, long long v);
__attribute__((hot)) TrStr LlvmEmitter_load_vreg(LlvmEmitter* self, long long v);
__attribute__((hot)) TrStr LlvmEmitter_load_vreg_as(LlvmEmitter* self, long long v, TrStr ty);
__attribute__((hot)) void LlvmEmitter_store_vreg(LlvmEmitter* self, long long v, TrStr ty, TrStr val);
__attribute__((hot)) TrStr LlvmEmitter_addr_of_base(LlvmEmitter* self, long long v);
__attribute__((hot)) TrStr LlvmEmitter_user_ret_ty(LlvmEmitter* self, TrStr name);
__attribute__((hot)) void LlvmEmitter_emit_inst(LlvmEmitter* self, LInst inst);
__attribute__((hot)) void LlvmEmitter_emit_call(LlvmEmitter* self, long long dst, TrStr callee, List_i64* args);
__attribute__((hot)) void LlvmEmitter_emit_term(LlvmEmitter* self, LTerm t);
__attribute__((hot)) void LlvmEmitter_emit_function(LlvmEmitter* self, LFunc* lf);
__attribute__((hot)) void LlvmEmitter_emit_extern_decls(LlvmEmitter* self);
__attribute__((hot)) void LlvmEmitter_scan_call_decl(LlvmEmitter* self, LInst inst, TrMap* seen);
__attribute__((hot)) TrStr LlvmEmitter_emit_module(LlvmEmitter* self);
__attribute__((hot)) TrStr _tbaa_suffix(long long aclass);
__attribute__((hot)) bool _is_fresh_alloc_ret(TrStr name);
__attribute__((hot)) TrStr _ll_int_instr(TrStr op);
__attribute__((hot)) TrStr _ll_icmp_pred(TrStr op);
__attribute__((hot)) TrStr _ll_float_instr(TrStr op);
__attribute__((hot)) TrStr _ll_fcmp_pred(TrStr op);
__attribute__((malloc,returns_nonnull,hot)) LlvmGenerator* LlvmGenerator_init();
__attribute__((hot)) TrStr LlvmGenerator_generate(LlvmGenerator* self, HirProgram* prog);
__attribute__((hot)) TrStr LlvmGenerator_generate_nh(LlvmGenerator* self, HirProgram* prog, bool no_heap);
__attribute__((malloc,returns_nonnull,hot)) ByteBuf* ByteBuf_init();
__attribute__((hot)) void ByteBuf_u8(ByteBuf* self, long long v);
__attribute__((hot)) void ByteBuf_u16(ByteBuf* self, long long v);
__attribute__((hot)) void ByteBuf_u32(ByteBuf* self, long long v);
__attribute__((hot)) void ByteBuf_u64(ByteBuf* self, long long v);
__attribute__((hot)) void ByteBuf_cstr(ByteBuf* self, TrStr s);
__attribute__((hot)) void ByteBuf_zeros(ByteBuf* self, long long n);
__attribute__((hot)) void ByteBuf_append_buf(ByteBuf* self, ByteBuf* o);
__attribute__((hot)) void ByteBuf_patch_u32(ByteBuf* self, long long off, long long v);
__attribute__((hot)) void ByteBuf_align_to(ByteBuf* self, long long align);
__attribute__((hot)) bool ByteBuf_write_file(ByteBuf* self, TrStr path);
__attribute__((malloc,returns_nonnull,hot)) EncodedFunc* EncodedFunc_init(TrStr name);
__attribute__((hot)) long long _argreg_modrm(long long idx);
__attribute__((hot)) long long _argreg_rex(long long idx);
__attribute__((hot)) long long _round16(long long n);
__attribute__((hot)) long long _vreg_disp(long long id);
__attribute__((hot)) long long _var_disp(LFunc* lf, TrStr name);
__attribute__((hot)) void _st_rax(ByteBuf* c, long long disp);
__attribute__((hot)) void _ld_rax(ByteBuf* c, long long disp);
__attribute__((hot)) void _ld_rcx(ByteBuf* c, long long disp);
__attribute__((hot)) void _ld_argreg(ByteBuf* c, long long idx, long long disp);
__attribute__((hot)) void _st_argreg(ByteBuf* c, long long idx, long long disp);
__attribute__((hot)) void _mov_rax_imm64(ByteBuf* c, long long v);
__attribute__((hot)) void _ld_xmm0(ByteBuf* c, long long disp);
__attribute__((hot)) void _ld_xmm1(ByteBuf* c, long long disp);
__attribute__((hot)) void _st_xmm0(ByteBuf* c, long long disp);
__attribute__((hot)) void _emit_farith(ByteBuf* c, TrStr op);
__attribute__((hot)) long long _fsetcc(TrStr op);
__attribute__((hot)) bool _is_cmp(TrStr op);
__attribute__((hot)) long long _setcc(TrStr op);
__attribute__((hot)) void _emit_arith(ByteBuf* c, TrStr op);
__attribute__((hot)) void _emit_return(ByteBuf* c, long long rv);
__attribute__((hot)) EncodedFunc* encode_func(LFunc* lf);
__attribute__((hot)) long long _index_of(List_TrStr* names, TrStr s);
__attribute__((hot)) void _shdr(ByteBuf* out, long long name, long long ty, long long flags, long long addr, long long offset, long long size, long long link, long long info, long long align, long long entsize);
__attribute__((hot)) long long _align8(long long n);
__attribute__((hot)) bool write_elf_object(TrStr out_path, List_ptr* funcs, List_TrStr* externs, List_TrStr* strings, long long n_globals);
__attribute__((hot)) bool emit_lir_object(LModule* m, TrStr out_path);
__attribute__((malloc,returns_nonnull,hot)) NativeGenerator* NativeGenerator_init();
__attribute__((hot)) bool NativeGenerator_emit_object(NativeGenerator* self, HirProgram* prog, TrStr out_path);
__attribute__((hot)) bool fn_is_kernel(HirFunction* f);
__attribute__((hot)) bool fn_is_device(HirFunction* f);
__attribute__((hot)) bool _gpu_is_float(TrStr t);
__attribute__((hot)) long long _gpu_iwidth(TrStr t);
__attribute__((hot)) TrStr _gpu_scalar_ty(TrStr n);
__attribute__((hot)) GVal* GVal_make(TrStr ty, TrStr val);
__attribute__((malloc,returns_nonnull,hot)) GpuEmitter* GpuEmitter_init(TrStr target);
__attribute__((hot)) TrStr GpuEmitter__gpu_ret_ty(GpuEmitter* self, AstType* t);
__attribute__((hot)) void GpuEmitter_w(GpuEmitter* self, TrStr s);
__attribute__((hot)) TrStr GpuEmitter_fresh(GpuEmitter* self);
__attribute__((hot)) TrStr GpuEmitter_newlbl(GpuEmitter* self);
__attribute__((hot)) void GpuEmitter_fail(GpuEmitter* self, TrStr why);
__attribute__((hot)) void GpuEmitter_add_var(GpuEmitter* self, TrStr name, AstType* ty);
__attribute__((hot)) TrStr GpuEmitter_var_slot_ty(GpuEmitter* self, TrStr name);
__attribute__((hot)) TrStr GpuEmitter_var_elem_ty(GpuEmitter* self, TrStr name);
__attribute__((hot)) void GpuEmitter_scan_vars_block(GpuEmitter* self, HirBlock* b);
__attribute__((hot)) void GpuEmitter_scan_vars_stmt(GpuEmitter* self, HirStmt s);
__attribute__((hot)) TrStr GpuEmitter_emit_kernel(GpuEmitter* self, HirFunction* f);
__attribute__((hot)) TrStr GpuEmitter_emit_device_fn(GpuEmitter* self, HirFunction* f);
__attribute__((hot)) void GpuEmitter_emit_block(GpuEmitter* self, HirBlock* b);
__attribute__((hot)) void GpuEmitter_emit_stmt(GpuEmitter* self, HirStmt s);
__attribute__((hot)) void GpuEmitter_emit_assign(GpuEmitter* self, HirExpr target, HirExpr* val);
__attribute__((hot)) void GpuEmitter_emit_stmt_expr(GpuEmitter* self, HirExpr e);
__attribute__((hot)) void GpuEmitter_emit_store(GpuEmitter* self, HirExpr chain, HirExpr* valp);
__attribute__((hot)) GVal* GpuEmitter_emit_ptr(GpuEmitter* self, HirExpr e);
__attribute__((hot)) GVal* GpuEmitter_emit_expr(GpuEmitter* self, HirExpr* ep);
__attribute__((hot)) GVal* GpuEmitter_emit_expr_hir(GpuEmitter* self, HirExpr e);
__attribute__((hot)) GVal* GpuEmitter_emit_load(GpuEmitter* self, HirExpr chain);
__attribute__((hot)) TrStr GpuEmitter_callee_name(GpuEmitter* self, HirExpr c);
__attribute__((hot)) GVal* GpuEmitter_emit_call(GpuEmitter* self, HirExpr callee, List_ptr* args);
__attribute__((hot)) GVal* GpuEmitter_emit_gpu_builtin(GpuEmitter* self, TrStr nm, long long dim);
__attribute__((hot)) GVal* GpuEmitter_i32_to_i64(GpuEmitter* self, TrStr v);
__attribute__((hot)) GVal* GpuEmitter_emit_binop(GpuEmitter* self, TrStr op, HirExpr* lp, HirExpr* rp);
__attribute__((hot)) TrStr GpuEmitter_cmp_pred(GpuEmitter* self, TrStr op, bool isf);
__attribute__((hot)) TrStr GpuEmitter_common_ty(GpuEmitter* self, TrStr a, TrStr b);
__attribute__((hot)) TrStr GpuEmitter_coerce(GpuEmitter* self, GVal* v, TrStr target);
__attribute__((hot)) TrStr GpuEmitter_coerce_bool(GpuEmitter* self, GVal* v);
__attribute__((hot)) long long GpuEmitter_literal_int(GpuEmitter* self, HirExpr e);
__attribute__((hot)) TrStr GpuEmitter_float_lit(GpuEmitter* self, double v);
__attribute__((hot)) long long _gpu_f64_bits(double v);
__attribute__((hot)) long long _hexdig(long long n);
__attribute__((hot)) TrStr _hex16(long long v);
__attribute__((malloc,returns_nonnull,hot)) GpuGenerator* GpuGenerator_init();
__attribute__((hot)) TrStr GpuGenerator_emit(GpuGenerator* self, HirProgram* prog, TrStr target);
__attribute__((hot)) MacroVal* box_mv(MacroVal v);
__attribute__((hot)) MacroVal* mrec(List_TrStr* keys, List_ptr* vals);
__attribute__((hot)) MacroVal* mrec_get(MacroVal* recptr, TrStr key);
__attribute__((hot)) TrStr mv_to_str(MacroVal* vptr);
__attribute__((hot)) bool mv_truthy(MacroVal* vptr);
__attribute__((hot)) bool mv_eq(MacroVal* a, MacroVal* b);
__attribute__((hot)) TrStr render_type(AstType** typtr);
__attribute__((hot)) MacroVal* str_list(List_TrStr* items);
__attribute__((hot)) MacroVal* param_rec(Param* p);
__attribute__((hot)) MacroVal* params_list(List_ptr* ps);
__attribute__((hot)) MacroVal* fn_rec(FunctionDef* f);
__attribute__((hot)) MacroVal* method_list(List_ptr* ms);
__attribute__((hot)) MacroVal* build_item(Decl* declptr);
__attribute__((malloc,returns_nonnull,hot)) MacroCtx* MacroCtx_init();
__attribute__((hot)) void MacroCtx_fail(MacroCtx* self, TrStr msg);
__attribute__((hot)) MacroVal* MacroCtx_eval_binop(MacroCtx* self, TrStr op, MacroVal* lv, MacroVal* rv);
__attribute__((hot)) MacroVal* MacroCtx_eval_mexpr(MacroCtx* self, Expr* eptr);
__attribute__((hot)) void MacroCtx_eval_mblock(MacroCtx* self, Block* b);
__attribute__((hot)) void MacroCtx_eval_mstmt(MacroCtx* self, Stmt* sptr);
__attribute__((hot)) void parse_into(TrStr src, List_ptr* gen);
__attribute__((hot)) List_ptr* decl_decorators(Decl* declptr);
__attribute__((hot)) TrStr render_arg(Expr* e);
__attribute__((hot)) Expr* parse_expr_src(TrStr src);
__attribute__((hot)) TrStr kind_of(Expr* e);
__attribute__((hot)) MacroVal* arg_rec(Expr* e);
__attribute__((hot)) MacroCtx* run_fn_macro(FunctionDef* mdef, List_ptr* args, long long gbase);
__attribute__((malloc,returns_nonnull,hot)) FnMacroExpander* FnMacroExpander_init(TrMap* m);
__attribute__((hot)) void FnMacroExpander_visit_expr(FnMacroExpander* self, Expr* eptr);
__attribute__((hot)) void FnMacroExpander_visit_block(FnMacroExpander* self, Block* b);
__attribute__((hot)) void FnMacroExpander_visit_stmt(FnMacroExpander* self, Stmt* sptr);
__attribute__((hot)) void FnMacroExpander_expand_decl(FnMacroExpander* self, Decl* dptr);
__attribute__((hot)) long long expand_macros(Program* prog);
__attribute__((hot)) TrStr gpu_blob_c(TrStr bin_path);
__attribute__((hot)) void print_version();
__attribute__((hot)) void print_usage();
__attribute__((hot)) bool str_ends_with_dot_tr(TrStr path);
__attribute__((hot)) TrStr strip_extension(TrStr path);
__attribute__((hot)) bool str_starts_with(TrStr s, TrStr prefix);
__attribute__((hot)) TrStr detect_c_compiler();
__attribute__((hot)) bool is_clang_compiler(TrStr cc);
__attribute__((hot)) TrStr pkg_config_query(TrStr name, TrStr kind);
__attribute__((hot)) TrStr cxx_of(TrStr cc);
__attribute__((hot)) bool cpp_lto_ok(TrStr cc, TrStr target);
__attribute__((hot)) bool auto_link_cpp_shims(List_TrStr* mod_file_paths, TrStr build_dir, TrStr cc, TrStr opt_level, List_TrStr* link_paths, List_TrStr* lib_flags, bool verbose, bool lto);
__attribute__((hot)) TrStr resolve_target_triple(TrStr target);
__attribute__((hot)) TrStr linker_script_cortex_m();
__attribute__((hot)) TrStr linker_script_riscv();
__attribute__((hot)) bool hir_has_export_fn(HirProgram* prog, TrStr name);
__attribute__((hot)) TrStr uefi_zig_stub(TrStr entry_name, bool has_heap_init, long long heap_size_mb);
__attribute__((hot)) TrStr uefi_zig_stub_frame(TrStr entry_name, bool has_heap_init, long long heap_size_mb);
__attribute__((hot)) TrStr target_extra_flags(TrStr triple);
__attribute__((hot)) TrStr detect_cross_compiler(TrStr triple);
__attribute__((hot)) TrStr detect_bundled_zig();
__attribute__((hot)) bool activate_bundled_zig();
__attribute__((hot)) TrStr detect_lld_flag();
__attribute__((hot)) TrStr zig_target_of(TrStr triple);
__attribute__((hot)) TrStr slice_after_arch(TrStr triple);
__attribute__((hot)) bool out_is_windows(TrStr target, TrStr triple);
__attribute__((hot)) long long str_len_of(TrStr s);
__attribute__((hot)) TrStr resolve_cross_driver(TrStr triple);
__attribute__((hot)) TrStr dir_of_path(TrStr path);
__attribute__((hot)) TrStr strip_trailing_sep_inline(TrStr s);
__attribute__((hot)) TrStr find_native_abi_c(TrStr input_path);
__attribute__((hot)) TrStr read_runtime_header(TrStr bin_path, TrStr input_path);
__attribute__((hot)) void ensure_runtime_header(TrStr out_dir, TrStr bin_path, TrStr input_path);
__attribute__((hot)) void sync_headers_to_runtime(TrStr rt_content, TrStr types_content);
__attribute__((hot)) TrStr strip_trailing_sep(TrStr s);
__attribute__((hot)) long long count_path_env_entries(TrStr s);
__attribute__((hot)) TrStr get_path_env_entry(TrStr s, long long idx);
__attribute__((hot)) TrStr to_runnable_path(TrStr s);
__attribute__((hot)) TrStr path_to_native(TrStr s);
__attribute__((hot)) TrStr to_fwd_slashes(TrStr s);
__attribute__((hot)) TrStr dot_to_safe(TrStr s);
__attribute__((hot)) TrStr dot_last_seg(TrStr s);
__attribute__((hot)) TrStr get_filename(TrStr path);
__attribute__((hot)) bool has_path_sep(TrStr path);
__attribute__((hot)) TrStr out_dir_prefix(TrStr out_dir);
__attribute__((hot)) TrStr resolve_output_stem(TrStr input_path, TrStr output_path, TrStr out_dir);
__attribute__((hot)) TrStr ascii_lower(TrStr s);
__attribute__((hot)) TrStr ensure_ext(TrStr path, TrStr ext);
__attribute__((hot)) long long get_dot_depth(TrStr dot_path);
__attribute__((hot)) TrStr ensure_builtin_dirs(TrStr build_dir, TrStr dot_path);
__attribute__((hot)) bool is_builtin_mod(TrStr dot_path);
__attribute__((hot)) void make_dir(TrStr path);
__attribute__((hot)) long long compile_all_c(List_TrStr* c_files, TrStr exe_path, TrStr inc_dir, List_TrStr* link_paths, List_TrStr* lib_flags, TrStr opt_level, bool verbose, bool static_link, TrStr target, TrStr sysroot, bool debug_mode);
__attribute__((hot)) TrStr obj_path_for(TrStr c_path);
__attribute__((hot)) long long compile_all_c_incremental(List_TrStr* c_files, List_bool* needs, TrStr exe_path, TrStr inc_dir, List_TrStr* link_paths, List_TrStr* lib_flags, TrStr opt_level, bool verbose, bool static_link, TrStr target, TrStr sysroot, bool debug_mode, bool build_shared, bool link_cxx, bool use_lto);
__attribute__((hot)) long long compile_c_to_exe(TrStr c_path, TrStr exe_path, TrStr opt_level, bool verbose);
__attribute__((hot)) void _print_diag(TrStr level, TrStr msg);
__attribute__((hot)) void cleanup_build(TrStr build_dir, List_TrStr* all_c_files);
__attribute__((hot)) void run_fmt(TrStr path0, bool write_in_place);

__attribute__((malloc,returns_nonnull,hot)) Lexer* Lexer_init(TrStr source);
__attribute__((hot)) void Lexer_push_loc(Lexer* self);
__attribute__((hot)) long long Lexer_peek(Lexer* self);
__attribute__((hot)) long long Lexer_peek_at(Lexer* self, long long offset);
__attribute__((hot)) bool Lexer__at_eol_after_ws(Lexer* self);
__attribute__((hot)) long long Lexer_advance(Lexer* self);
__attribute__((hot)) bool Lexer_at_end(Lexer* self);
__attribute__((hot)) void Lexer_skip_spaces(Lexer* self);
__attribute__((hot)) void Lexer_skip_comment(Lexer* self, bool trailing);
__attribute__((hot)) Token Lexer_read_int(Lexer* self);
__attribute__((hot)) Token Lexer_read_triple_string(Lexer* self, long long quote);
__attribute__((hot)) Token Lexer_read_string(Lexer* self, long long quote);
__attribute__((hot)) Token Lexer_read_char(Lexer* self);
__attribute__((hot)) Token Lexer_read_fstring(Lexer* self);
__attribute__((hot)) Token Lexer_read_triple_fstring(Lexer* self, long long quote);
__attribute__((hot)) Token Lexer_read_raw_string(Lexer* self);
__attribute__((hot)) Token Lexer_read_byte_string(Lexer* self);
__attribute__((hot)) Token Lexer_read_ident(Lexer* self);
__attribute__((hot)) List_Token* Lexer_tokenize(Lexer* self);
__attribute__((malloc,returns_nonnull,hot)) CGenerator* CGenerator_init();
__attribute__((hot)) TrStr CGenerator_next_temp(CGenerator* self);
__attribute__((hot)) void CGenerator_reset_defer_stack(CGenerator* self);
__attribute__((hot)) void CGenerator_gen_func_body(CGenerator* self, HirBlock* body, long long indent);
__attribute__((hot)) void CGenerator_flush_closures(CGenerator* self);
__attribute__((hot)) void CGenerator_seed_params(CGenerator* self, HirFunction* f);
__attribute__((hot)) void CGenerator_w(CGenerator* self, TrStr s);
__attribute__((hot)) void CGenerator_wf(CGenerator* self, TrStr s);
__attribute__((hot)) void CGenerator_ws(CGenerator* self, TrStr s);
__attribute__((hot)) void CGenerator_wp(CGenerator* self, TrStr s);
__attribute__((hot)) void CGenerator_wlt(CGenerator* self, TrStr s);
__attribute__((hot)) void CGenerator_ensure_list_type(CGenerator* self, TrStr n);
__attribute__((hot)) void CGenerator_ensure_dict_type(CGenerator* self, TrStr kc, TrStr vname);
__attribute__((hot)) void CGenerator_ensure_set_type(CGenerator* self, TrStr vname);
__attribute__((hot)) void CGenerator_check_and_emit_list_fwd(CGenerator* self, AstType* ty);
__attribute__((hot)) void CGenerator_emit_list_fwd_decls(CGenerator* self, HirProgram* prog);
__attribute__((hot)) TrStr CGenerator_type_to_c(CGenerator* self, AstType* ty);
__attribute__((hot)) TrStr CGenerator_type_suffix(CGenerator* self, TrStr n);
__attribute__((hot)) TrStr CGenerator_list_elem_suffix(CGenerator* self, TrStr n);
__attribute__((hot)) TrStr CGenerator_list_sfx(CGenerator* self, TrStr elem_sfx);
__attribute__((hot)) TrStr CGenerator_type_args_suffix(CGenerator* self, List_ptr* args);
__attribute__((hot)) TrStr CGenerator_synth_class_suffix(CGenerator* self, HirClass* ucls);
__attribute__((hot)) TrStr CGenerator_ensure_array_type(CGenerator* self, AstType* ty);
__attribute__((hot)) TrStr CGenerator_mono_cprefix(CGenerator* self, AstType* t);
__attribute__((hot)) void CGenerator_capture_local_mono(CGenerator* self, TrStr name, AstType* ty, HirExpr* v);
__attribute__((hot)) void CGenerator_ensure_mono(CGenerator* self, HirClass* cls, List_ptr* type_args);
__attribute__((hot)) TrStr CGenerator_infer_generic_targ(CGenerator* self, TrStr fname, List_ptr* args);
__attribute__((hot)) List_TrStr* CGenerator_infer_generic_targs_multi(CGenerator* self, TrStr fname, List_ptr* args);
__attribute__((hot)) void CGenerator_ensure_mono_func(CGenerator* self, TrStr fname, TrStr targ);
__attribute__((hot)) void CGenerator_ensure_mono_func_n(CGenerator* self, TrStr fname, List_TrStr* targs);
__attribute__((hot)) TrStr CGenerator_infer_method_targ(CGenerator* self, TrStr cls_name, TrStr method, List_ptr* args);
__attribute__((hot)) void CGenerator_ensure_mono_method(CGenerator* self, TrStr cls_name, TrStr method, TrStr targ);
__attribute__((hot)) TrStr CGenerator_get_user_decorator_attr(CGenerator* self, TrStr name);
__attribute__((hot)) TrStr CGenerator_deco_first_str(CGenerator* self, Decorator* d);
__attribute__((hot)) long long CGenerator_deco_first_int(CGenerator* self, Decorator* d);
__attribute__((hot)) TrStr CGenerator_struct_layout(CGenerator* self, HirClass* c);
__attribute__((hot)) TrStr CGenerator_struct_kw(CGenerator* self, HirClass* c);
__attribute__((hot)) TrStr CGenerator_hw_attrs(CGenerator* self, HirFunction* f);
__attribute__((hot)) TrStr CGenerator_hook_macro_for(CGenerator* self, HirFunction* f);
__attribute__((hot)) TrStr CGenerator_emit_tier_hooks(CGenerator* self, HirProgram* prog);
__attribute__((hot)) void CGenerator_emit_global_inits(CGenerator* self, HirProgram* prog);
__attribute__((hot)) void CGenerator_emit_entry_glue(CGenerator* self, HirProgram* prog);
__attribute__((hot)) TrStr CGenerator_get_inline_attrs(CGenerator* self, HirFunction* f);
__attribute__((hot)) bool CGenerator_is_rt_concurrency_type(CGenerator* self, TrStr name);
__attribute__((hot)) TrStr CGenerator_get_proto_attrs(CGenerator* self, HirFunction* f);
__attribute__((hot)) bool CGenerator_vt_method_mutates_self(CGenerator* self, HirFunction* f);
__attribute__((hot)) bool CGenerator_vt_call_needs_ptr_self(CGenerator* self, TrStr class_name, TrStr method);
__attribute__((hot)) bool CGenerator_hir_block_mutates_self(CGenerator* self, HirBlock* block);
__attribute__((hot)) bool CGenerator_hir_stmt_mutates_self(CGenerator* self, HirStmt* s);
__attribute__((hot)) TrStr CGenerator_gen_func_sig(CGenerator* self, HirFunction* f, TrStr class_name);
__attribute__((hot)) void CGenerator_emit_base_fields(CGenerator* self, TrStr base_name);
__attribute__((hot)) void CGenerator_emit_drop_fwd_decls(CGenerator* self, HirProgram* prog);
__attribute__((hot)) void CGenerator_gen_class_struct(CGenerator* self, HirClass* c);
__attribute__((hot)) void CGenerator_gen_enum_struct(CGenerator* self, HirEnum* e);
__attribute__((hot)) void CGenerator_gen_interface_vtable(CGenerator* self, HirInterface* iface);
__attribute__((hot)) TrStr CGenerator_gen_one_iface_wrap(CGenerator* self, TrStr cls_name, HirInterface* iface, List_ptr* targs);
__attribute__((hot)) TrStr CGenerator_gen_expr(CGenerator* self, HirExpr* e_ptr);
__attribute__((hot)) TrStr CGenerator_opt_payload_binding(CGenerator* self, AstType* subj_hty, TrStr tn, TrStr vn, TrStr bind_c, TrStr slot);
__attribute__((hot)) TrStr CGenerator_gen_match_expr(CGenerator* self, HirExpr* subj, List_ptr* arms, AstType* ty);
__attribute__((hot)) bool CGenerator_has_method(CGenerator* self, TrStr cls_name, TrStr method);
__attribute__((hot)) AstType* CGenerator_cls_method_ret_ty(CGenerator* self, TrStr cls_name, TrStr method);
__attribute__((hot)) TrStr CGenerator_cls_method_c_call(CGenerator* self, TrStr cls_name, TrStr method, TrStr obj_s, TrStr extra_args);
__attribute__((hot)) TrStr CGenerator_resolve_generic_prim(CGenerator* self, TrStr n);
__attribute__((hot)) TrStr CGenerator_resolve_generic_tyname(CGenerator* self, TrStr n);
__attribute__((hot)) TrStr CGenerator_mono_cls_name_for(CGenerator* self, AstType* ty);
__attribute__((hot)) TrStr CGenerator_obj_to_str_expr(CGenerator* self, TrStr mono, TrStr s);
__attribute__((hot)) TrStr CGenerator_ensure_elem_fmt_fn(CGenerator* self, AstType* ty);
__attribute__((hot)) TrStr CGenerator_gen_collection_to_str(CGenerator* self, TrStr s, AstType* ty);
__attribute__((hot)) TrStr CGenerator_strz(CGenerator* self, TrStr e);
__attribute__((hot)) TrStr CGenerator_flush_wraps(CGenerator* self, TrStr expr_s, bool is_void);
__attribute__((hot)) TrStr CGenerator_wrapstr(CGenerator* self, TrStr e);
__attribute__((hot)) void CGenerator_set_proven_borrows(CGenerator* self, List_TrStr* pb);
__attribute__((hot)) TrStr CGenerator_str_retain_wrap(CGenerator* self, HirExpr* e, TrStr s, bool is_return);
__attribute__((hot)) void CGenerator__scan_ptr_aliased(CGenerator* self, AstType* t);
__attribute__((hot)) bool CGenerator__is_coll_type_name(CGenerator* self, TrStr n);
__attribute__((hot)) bool CGenerator__coll_init_is_borrow(CGenerator* self, HirExpr* v);
__attribute__((hot)) bool CGenerator__is_fresh_coll_expr(CGenerator* self, HirExpr* e);
__attribute__((hot)) void CGenerator__scan_coll_fields(CGenerator* self, HirProgram* prog);
__attribute__((hot)) void CGenerator__scan_coll_fields_block(CGenerator* self, HirBlock* b);
__attribute__((hot)) void CGenerator__scan_coll_fields_stmt(CGenerator* self, HirStmt* sp);
__attribute__((hot)) TrStr CGenerator__coll_field_free_call(CGenerator* self, TrStr fld_c, AstType* ty);
__attribute__((hot)) bool CGenerator_is_heap_class_tn(CGenerator* self, TrStr tn);
__attribute__((hot)) TrStr CGenerator_iface_wrap_return(CGenerator* self, HirExpr* e, TrStr s);
__attribute__((hot)) TrStr CGenerator_iface_wrap_to(CGenerator* self, AstType* target_ty, HirExpr* e, TrStr s);
__attribute__((hot)) TrStr CGenerator_obj_retain_wrap(CGenerator* self, HirExpr* e, TrStr s, bool is_return);
__attribute__((hot)) bool CGenerator__fn_owned_lookup(CGenerator* self, TrStr key);
__attribute__((hot)) void CGenerator__reg_fn_owned(CGenerator* self, TrStr key, bool v);
__attribute__((hot)) bool CGenerator__obj_expr_owns_ref(CGenerator* self, HirExpr* e);
__attribute__((hot)) bool CGenerator__store_target_is_borrow_field(CGenerator* self, HirExpr* t);
__attribute__((hot)) bool CGenerator__obj_store_needs_retain(CGenerator* self, HirExpr* e);
__attribute__((hot)) TrStr CGenerator_obj_drop_fn(CGenerator* self, TrStr tn);
__attribute__((hot)) TrStr CGenerator__mtxcd_spec(CGenerator* self, AstType** ptp);
__attribute__((hot)) TrStr CGenerator__mtxcd_name_from_spec(CGenerator* self, TrStr spec);
__attribute__((hot)) TrStr CGenerator__mtxcd_body_from_spec(CGenerator* self, TrStr spec);
__attribute__((hot)) TrStr CGenerator__mtxcd_name(CGenerator* self, AstType** ptp);
__attribute__((hot)) void CGenerator_emit_mtxcd_wrappers(CGenerator* self);
__attribute__((hot)) bool CGenerator__mutex_payload_fresh_coll(CGenerator* self, HirExpr* e);
__attribute__((hot)) TrStr CGenerator_gen_cond_expr(CGenerator* self, HirExpr* cond);
__attribute__((hot)) TrStr CGenerator_gen_binop(CGenerator* self, TrStr op, HirExpr* l, HirExpr* r);
__attribute__((hot)) TrStr CGenerator_gen_unary(CGenerator* self, TrStr op, HirExpr* expr);
__attribute__((hot)) TrStr CGenerator_gen_prop_access(CGenerator* self, HirExpr* o, TrStr p);
__attribute__((hot)) TrStr CGenerator_gen_index(CGenerator* self, HirExpr* o, HirExpr* idx);
__attribute__((hot)) bool CGenerator__call_arg_needs_shared_unwrap(CGenerator* self, HirExpr* arg, AstType* param_ty);
__attribute__((hot)) TrStr CGenerator__gpu_arg_kind(CGenerator* self, AstType* pty);
__attribute__((hot)) TrStr CGenerator_gen_kernel_launch(CGenerator* self, List_ptr* args);
__attribute__((hot)) TrStr CGenerator_gen_call(CGenerator* self, HirExpr* callee, List_ptr* args, AstType* call_ty);
__attribute__((hot)) TrStr CGenerator_gen_print_call(CGenerator* self, List_ptr* args);
__attribute__((hot)) TrStr CGenerator_gen_to_cstr(CGenerator* self, HirExpr* arg);
__attribute__((hot)) TrStr CGenerator_gen_print_one(CGenerator* self, HirExpr* arg);
__attribute__((hot)) TrStr CGenerator_wrap_voidp_arg(CGenerator* self, HirExpr* arg);
__attribute__((hot)) TrStr CGenerator_unwrap_voidp_as(CGenerator* self, TrStr expr_s, AstType* ty);
__attribute__((hot)) TrStr CGenerator_dict_key_arg(CGenerator* self, HirExpr* e);
__attribute__((hot)) TrStr CGenerator_dict_val_arg(CGenerator* self, HirExpr* e);
__attribute__((hot)) TrStr CGenerator_gen_args_extern(CGenerator* self, List_ptr* args);
__attribute__((hot)) bool CGenerator__is_fresh_str_expr(CGenerator* self, HirExpr* e);
__attribute__((hot)) bool CGenerator__expr_is_borrow_call(CGenerator* self, HirExpr* e);
__attribute__((hot)) bool CGenerator__is_fresh_obj_expr(CGenerator* self, HirExpr* e);
__attribute__((hot)) TrStr CGenerator_gen_args(CGenerator* self, List_ptr* args);
__attribute__((hot)) bool CGenerator__is_subclass_of(CGenerator* self, TrStr child, TrStr parent);
__attribute__((hot)) TrStr CGenerator_gen_args_casts(CGenerator* self, List_ptr* args, List_TrStr* casts);
__attribute__((hot)) TrStr CGenerator_gen_args_strify(CGenerator* self, List_ptr* args, TrStr elem_sfx);
__attribute__((hot)) TrStr CGenerator__resolve_alias_tr(CGenerator* self, TrStr name);
__attribute__((hot)) TrStr CGenerator_gen_iface_wrapped_args(CGenerator* self, List_ptr* params, List_ptr* args);
__attribute__((hot)) TrStr CGenerator_gen_method_call(CGenerator* self, HirExpr* obj, TrStr method, List_ptr* args, AstType* call_ty);
__attribute__((hot)) TrStr CGenerator_gen_fstring(CGenerator* self, List_ptr* parts);
__attribute__((hot)) TrStr CGenerator_gen_tuple(CGenerator* self, List_ptr* items);
__attribute__((hot)) TrStr CGenerator_gen_tuple_to_str(CGenerator* self, TrStr s, AstType* ty);
__attribute__((hot)) void CGenerator_emit_async_wrapper_for_call(CGenerator* self, TrStr fn_name, List_ptr* args, TrStr ret_name);
__attribute__((hot)) TrStr CGenerator_gen_await_call(CGenerator* self, HirExpr* expr);
__attribute__((hot)) TrStr CGenerator_gen_await_timeout_call(CGenerator* self, HirExpr* expr, HirExpr* ms_expr);
__attribute__((hot)) void CGenerator_gen_multi_let(CGenerator* self, List_TrStr* names, bool is_mut, HirExpr* val, long long indent);
__attribute__((hot)) TrStr CGenerator_gen_list_literal(CGenerator* self, List_ptr* items, AstType* ty);
__attribute__((hot)) TrStr CGenerator_gen_dict_literal(CGenerator* self, List_ptr* keys, List_ptr* vals, AstType* hint_ty);
__attribute__((hot)) TrStr CGenerator_gen_list_comp(CGenerator* self, HirExpr* element, List_ptr* generators);
__attribute__((hot)) TrStr CGenerator__comp_src_free_stmt(CGenerator* self, HirExpr* iter_e, long long idx);
__attribute__((hot)) TrStr CGenerator_cb_trampoline(CGenerator* self, AstType* sig);
__attribute__((hot)) TrStr CGenerator_gen_closure(CGenerator* self, List_ptr* params, AstType* ret_ty, HirBlock* body, List_ptr* captures);
__attribute__((hot)) TrStr CGenerator__spawn_wrap_cast_ty(CGenerator* self, TrStr fn_name, HirExpr* arg_expr);
__attribute__((hot)) void CGenerator_emit_spawn_wrapper_for_expr(CGenerator* self, HirExpr* e);
__attribute__((hot)) void CGenerator_prescan_block_spawns(CGenerator* self, HirBlock* block);
__attribute__((hot)) void CGenerator_prescan_stmt_spawns(CGenerator* self, HirStmt* s);
__attribute__((hot)) void CGenerator_prescan_await_all_in_expr(CGenerator* self, HirExpr* e);
__attribute__((hot)) void CGenerator_prescan_spawns(CGenerator* self, HirProgram* prog);
__attribute__((hot)) void CGenerator_prescan_expr_awaits(CGenerator* self, HirExpr* e);
__attribute__((hot)) void CGenerator_prescan_block_awaits(CGenerator* self, HirBlock* block);
__attribute__((hot)) void CGenerator_prescan_stmt_awaits(CGenerator* self, HirStmt* s);
__attribute__((hot)) void CGenerator_prescan_awaits(CGenerator* self, HirProgram* prog);
__attribute__((hot)) void CGenerator_gen_stmt(CGenerator* self, HirStmt* s_ptr, long long indent);
__attribute__((hot)) void CGenerator_gen_for_loop(CGenerator* self, TrStr var, HirExpr* iter, HirBlock* body, long long indent);
__attribute__((hot)) void CGenerator_gen_for_unpack(CGenerator* self, List_TrStr* vars, HirExpr* iter, HirBlock* body, long long indent);
__attribute__((hot)) void CGenerator_gen_try(CGenerator* self, HirBlock* try_body, List_ptr* catches, HirBlock* finally_b, long long indent);
__attribute__((hot)) void CGenerator_gen_chan_select(CGenerator* self, List_ptr* arms, long long indent);
__attribute__((hot)) void CGenerator_gen_block(CGenerator* self, HirBlock* b, long long indent);
__attribute__((hot)) void CGenerator_gen_match(CGenerator* self, HirExpr* expr, List_ptr* arms, long long indent);
__attribute__((hot)) void CGenerator_register_program(CGenerator* self, HirProgram* prog);
__attribute__((hot)) void CGenerator_scan_mono_ty(CGenerator* self, AstType* ty);
__attribute__((hot)) void CGenerator_scan_mono_block(CGenerator* self, HirBlock* block);
__attribute__((hot)) void CGenerator_scan_mono_expr(CGenerator* self, HirExpr* e);
__attribute__((hot)) void CGenerator_scan_mono_stmt(CGenerator* self, HirStmt* s_ptr);
__attribute__((hot)) void CGenerator_scan_mono_func(CGenerator* self, HirFunction* f);
__attribute__((hot)) void CGenerator_scan_mono_prog(CGenerator* self, HirProgram* prog);
__attribute__((hot)) TrStr CGenerator_generate(CGenerator* self, HirProgram* prog);
__attribute__((hot)) TrStr CGenerator_generate_types_header(CGenerator* self, HirProgram* prog);
__attribute__((hot)) TrStr CGenerator_generate_module_compat(CGenerator* self, List_TrStr* all_decl_modules, List_ptr* all_decls);
__attribute__((hot)) TrStr CGenerator_generate_module_c(CGenerator* self, HirProgram* prog, TrMap* class_set, TrMap* fn_set, long long depth);
__attribute__((hot)) TrStr CGenerator_generate_export_header(CGenerator* self, HirProgram* prog);
__attribute__((hot)) TrStr CGenerator_generate_main_c(CGenerator* self, HirProgram* prog, TrMap* class_set, TrMap* fn_set);
__attribute__((malloc,returns_nonnull,hot)) CppExporter* CppExporter_init(CGenerator* gen);
__attribute__((hot)) TrStr CppExporter__cpp_ty_kind(CppExporter* self, AstType* ty);
__attribute__((hot)) bool CppExporter__cpp_list_ok(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_list_elem_kind(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_list_sfx(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_list_elem_c(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_list_elem_cpp(CppExporter* self, AstType* ty);
__attribute__((hot)) bool CppExporter__cpp_slot_ok(CppExporter* self, TrStr name);
__attribute__((hot)) TrStr CppExporter__cpp_slot_kind(CppExporter* self, TrStr name);
__attribute__((hot)) TrStr CppExporter__cpp_slot_c(CppExporter* self, TrStr kind);
__attribute__((hot)) TrStr CppExporter__cpp_slot_cpp(CppExporter* self, TrStr kind);
__attribute__((hot)) bool CppExporter__cpp_dict_ok(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_dict_kkind(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_dict_vkind(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_dict_variant(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_dict_key_cpp(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_dict_v_cpp(CppExporter* self, AstType* ty);
__attribute__((hot)) bool CppExporter__cpp_tuple_ok(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_tuple_elem_cpp(CppExporter* self, AstType* ty, long long i);
__attribute__((hot)) void CppExporter__note_export_ty(CppExporter* self, AstType* ty);
__attribute__((hot)) void CppExporter_collect_export_classes(CppExporter* self, HirProgram* prog);
__attribute__((hot)) bool CppExporter__cpp_export_ty_ok(CppExporter* self, AstType* ty);
__attribute__((hot)) bool CppExporter__cpp_is_class_list(CppExporter* self, AstType* ty);
__attribute__((hot)) bool CppExporter__cpp_export_fn_ok(CppExporter* self, HirFunction* f);
__attribute__((hot)) bool CppExporter__cpp_kind_bridged(CppExporter* self, TrStr k);
__attribute__((hot)) bool CppExporter__cpp_fn_needs_bridge(CppExporter* self, HirFunction* f);
__attribute__((hot)) bool CppExporter__cpp_method_ok(CppExporter* self, HirFunction* m);
__attribute__((hot)) TrStr CppExporter__cpp_tuple_type_cpp(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_c_ty(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_c_arg(CppExporter* self, AstType* ty, TrStr name);
__attribute__((hot)) TrStr CppExporter__cpp_param_decl(CppExporter* self, AstType* ty, TrStr name);
__attribute__((hot)) TrStr CppExporter__cpp_call_arg(CppExporter* self, AstType* ty, TrStr name);
__attribute__((hot)) TrStr CppExporter__cpp_ret_type(CppExporter* self, AstType* ty);
__attribute__((hot)) TrStr CppExporter__cpp_ret_stmt(CppExporter* self, AstType* ty, TrStr call);
__attribute__((hot)) bool CppExporter__cpp_kind_marshal(CppExporter* self, TrStr k);
__attribute__((hot)) bool CppExporter__cpp_fn_has_list(CppExporter* self, HirFunction* f);
__attribute__((hot)) TrStr CppExporter__cpp_wrap_fn(CppExporter* self, HirFunction* f);
__attribute__((hot)) bool CppExporter__cpp_init_ok(CppExporter* self, HirFunction* m);
__attribute__((hot)) long long CppExporter__cpp_init_idx(CppExporter* self, HirClass* c);
__attribute__((hot)) TrStr CppExporter__cpp_class_decl(CppExporter* self, HirClass* c);
__attribute__((hot)) TrStr CppExporter_generate_cpp_class_bridges(CppExporter* self, HirProgram* prog);
__attribute__((hot)) TrStr CppExporter__cpp_list_bridges(CppExporter* self);
__attribute__((hot)) TrStr CppExporter_generate_export_cpp_header(CppExporter* self, HirProgram* prog, TrStr ns);
__attribute__((hot)) TrStr CppExporter__cpp_class_bridge_protos(CppExporter* self, HirClass* c);
__attribute__((hot)) TrStr CppExporter__cpp_free_fn_bridge_proto(CppExporter* self, HirFunction* f);


/* === Module-prefixed typedef aliases (auto-generated) === */
/* Maps module-qualified C names to short-name types in tauraro_types.h */

#ifndef TAURARO_RT_NO_STRINGBUILDER
typedef struct core_string_StringObj core_string_StringObj;
typedef core_string_StringObj StringObj;
typedef struct core_string_StringBuilder core_string_StringBuilder;
typedef core_string_StringBuilder StringBuilder;
#endif

typedef StringObj core_string_StringObj;
struct core_vec_Vec_core_string_StringObj { core_string_StringObj** data; long long len; long long capacity; };
typedef struct core_vec_Vec_core_string_StringObj core_vec_Vec_core_string_StringObj;
struct core_vec_Vec_core_string_StringObj_ptr { core_string_StringObj*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_core_string_StringObj_ptr core_vec_Vec_core_string_StringObj_ptr;
struct core_map_MapNode_str_core_string_StringObj { char* key; core_string_StringObj* value; struct core_map_MapNode_str_core_string_StringObj* next; };
typedef struct core_map_MapNode_str_core_string_StringObj core_map_MapNode_str_core_string_StringObj;
struct core_map_Map_str_core_string_StringObj { core_map_MapNode_str_core_string_StringObj** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_core_string_StringObj core_map_Map_str_core_string_StringObj;
__attribute__((hot)) core_string_StringObj** core_alloc_alloc_core_string_StringObj(long long count);
__attribute__((hot)) core_string_StringObj** core_alloc_resize_core_string_StringObj(core_string_StringObj** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_core_string_StringObj(core_string_StringObj** ptr);
__attribute__((hot)) core_string_StringObj*** core_alloc_alloc_core_string_StringObj_ptr(long long count);
__attribute__((hot)) core_string_StringObj*** core_alloc_resize_core_string_StringObj_ptr(core_string_StringObj*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_core_string_StringObj_ptr(core_string_StringObj*** ptr);
__attribute__((hot)) core_map_MapNode_str_core_string_StringObj** core_alloc_alloc_core_map_MapNode_str_core_string_StringObj(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_core_string_StringObj(core_map_MapNode_str_core_string_StringObj** ptr);

typedef StringBuilder core_string_StringBuilder;
struct core_vec_Vec_core_string_StringBuilder { core_string_StringBuilder** data; long long len; long long capacity; };
typedef struct core_vec_Vec_core_string_StringBuilder core_vec_Vec_core_string_StringBuilder;
struct core_vec_Vec_core_string_StringBuilder_ptr { core_string_StringBuilder*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_core_string_StringBuilder_ptr core_vec_Vec_core_string_StringBuilder_ptr;
struct core_map_MapNode_str_core_string_StringBuilder { char* key; core_string_StringBuilder* value; struct core_map_MapNode_str_core_string_StringBuilder* next; };
typedef struct core_map_MapNode_str_core_string_StringBuilder core_map_MapNode_str_core_string_StringBuilder;
struct core_map_Map_str_core_string_StringBuilder { core_map_MapNode_str_core_string_StringBuilder** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_core_string_StringBuilder core_map_Map_str_core_string_StringBuilder;
__attribute__((hot)) core_string_StringBuilder** core_alloc_alloc_core_string_StringBuilder(long long count);
__attribute__((hot)) core_string_StringBuilder** core_alloc_resize_core_string_StringBuilder(core_string_StringBuilder** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_core_string_StringBuilder(core_string_StringBuilder** ptr);
__attribute__((hot)) core_string_StringBuilder*** core_alloc_alloc_core_string_StringBuilder_ptr(long long count);
__attribute__((hot)) core_string_StringBuilder*** core_alloc_resize_core_string_StringBuilder_ptr(core_string_StringBuilder*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_core_string_StringBuilder_ptr(core_string_StringBuilder*** ptr);
__attribute__((hot)) core_map_MapNode_str_core_string_StringBuilder** core_alloc_alloc_core_map_MapNode_str_core_string_StringBuilder(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_core_string_StringBuilder(core_map_MapNode_str_core_string_StringBuilder** ptr);

typedef Token token_Token;
struct core_vec_Vec_token_Token { token_Token* data; long long len; long long capacity; };
typedef struct core_vec_Vec_token_Token core_vec_Vec_token_Token;
struct core_vec_Vec_token_Token_ptr { token_Token** data; long long len; long long capacity; };
typedef struct core_vec_Vec_token_Token_ptr core_vec_Vec_token_Token_ptr;
__attribute__((hot)) token_Token* core_alloc_alloc_token_Token(long long count);
__attribute__((hot)) token_Token* core_alloc_resize_token_Token(token_Token* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_token_Token(token_Token* ptr);
__attribute__((hot)) token_Token** core_alloc_alloc_token_Token_ptr(long long count);
__attribute__((hot)) token_Token** core_alloc_resize_token_Token_ptr(token_Token** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_token_Token_ptr(token_Token** ptr);

typedef Lexer lexer_Lexer;
struct core_vec_Vec_lexer_Lexer { lexer_Lexer** data; long long len; long long capacity; };
typedef struct core_vec_Vec_lexer_Lexer core_vec_Vec_lexer_Lexer;
struct core_vec_Vec_lexer_Lexer_ptr { lexer_Lexer*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_lexer_Lexer_ptr core_vec_Vec_lexer_Lexer_ptr;
struct core_map_MapNode_str_lexer_Lexer { char* key; lexer_Lexer* value; struct core_map_MapNode_str_lexer_Lexer* next; };
typedef struct core_map_MapNode_str_lexer_Lexer core_map_MapNode_str_lexer_Lexer;
struct core_map_Map_str_lexer_Lexer { core_map_MapNode_str_lexer_Lexer** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_lexer_Lexer core_map_Map_str_lexer_Lexer;
__attribute__((hot)) lexer_Lexer** core_alloc_alloc_lexer_Lexer(long long count);
__attribute__((hot)) lexer_Lexer** core_alloc_resize_lexer_Lexer(lexer_Lexer** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_lexer_Lexer(lexer_Lexer** ptr);
__attribute__((hot)) lexer_Lexer*** core_alloc_alloc_lexer_Lexer_ptr(long long count);
__attribute__((hot)) lexer_Lexer*** core_alloc_resize_lexer_Lexer_ptr(lexer_Lexer*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_lexer_Lexer_ptr(lexer_Lexer*** ptr);
__attribute__((hot)) core_map_MapNode_str_lexer_Lexer** core_alloc_alloc_core_map_MapNode_str_lexer_Lexer(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_lexer_Lexer(core_map_MapNode_str_lexer_Lexer** ptr);

typedef AstType ast_AstType;
struct core_vec_Vec_ast_AstType { ast_AstType** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_AstType core_vec_Vec_ast_AstType;
struct core_vec_Vec_ast_AstType_ptr { ast_AstType*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_AstType_ptr core_vec_Vec_ast_AstType_ptr;
struct core_map_MapNode_str_ast_AstType { char* key; ast_AstType* value; struct core_map_MapNode_str_ast_AstType* next; };
typedef struct core_map_MapNode_str_ast_AstType core_map_MapNode_str_ast_AstType;
struct core_map_Map_str_ast_AstType { core_map_MapNode_str_ast_AstType** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_AstType core_map_Map_str_ast_AstType;
__attribute__((hot)) ast_AstType** core_alloc_alloc_ast_AstType(long long count);
__attribute__((hot)) ast_AstType** core_alloc_resize_ast_AstType(ast_AstType** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_AstType(ast_AstType** ptr);
__attribute__((hot)) ast_AstType*** core_alloc_alloc_ast_AstType_ptr(long long count);
__attribute__((hot)) ast_AstType*** core_alloc_resize_ast_AstType_ptr(ast_AstType*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_AstType_ptr(ast_AstType*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_AstType** core_alloc_alloc_core_map_MapNode_str_ast_AstType(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_AstType(core_map_MapNode_str_ast_AstType** ptr);

typedef GenericConstraint ast_GenericConstraint;
struct core_vec_Vec_ast_GenericConstraint { ast_GenericConstraint** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_GenericConstraint core_vec_Vec_ast_GenericConstraint;
struct core_vec_Vec_ast_GenericConstraint_ptr { ast_GenericConstraint*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_GenericConstraint_ptr core_vec_Vec_ast_GenericConstraint_ptr;
struct core_map_MapNode_str_ast_GenericConstraint { char* key; ast_GenericConstraint* value; struct core_map_MapNode_str_ast_GenericConstraint* next; };
typedef struct core_map_MapNode_str_ast_GenericConstraint core_map_MapNode_str_ast_GenericConstraint;
struct core_map_Map_str_ast_GenericConstraint { core_map_MapNode_str_ast_GenericConstraint** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_GenericConstraint core_map_Map_str_ast_GenericConstraint;
__attribute__((hot)) ast_GenericConstraint** core_alloc_alloc_ast_GenericConstraint(long long count);
__attribute__((hot)) ast_GenericConstraint** core_alloc_resize_ast_GenericConstraint(ast_GenericConstraint** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_GenericConstraint(ast_GenericConstraint** ptr);
__attribute__((hot)) ast_GenericConstraint*** core_alloc_alloc_ast_GenericConstraint_ptr(long long count);
__attribute__((hot)) ast_GenericConstraint*** core_alloc_resize_ast_GenericConstraint_ptr(ast_GenericConstraint*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_GenericConstraint_ptr(ast_GenericConstraint*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_GenericConstraint** core_alloc_alloc_core_map_MapNode_str_ast_GenericConstraint(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_GenericConstraint(core_map_MapNode_str_ast_GenericConstraint** ptr);

typedef Decorator ast_Decorator;
struct core_vec_Vec_ast_Decorator { ast_Decorator** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Decorator core_vec_Vec_ast_Decorator;
struct core_vec_Vec_ast_Decorator_ptr { ast_Decorator*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Decorator_ptr core_vec_Vec_ast_Decorator_ptr;
struct core_map_MapNode_str_ast_Decorator { char* key; ast_Decorator* value; struct core_map_MapNode_str_ast_Decorator* next; };
typedef struct core_map_MapNode_str_ast_Decorator core_map_MapNode_str_ast_Decorator;
struct core_map_Map_str_ast_Decorator { core_map_MapNode_str_ast_Decorator** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_Decorator core_map_Map_str_ast_Decorator;
__attribute__((hot)) ast_Decorator** core_alloc_alloc_ast_Decorator(long long count);
__attribute__((hot)) ast_Decorator** core_alloc_resize_ast_Decorator(ast_Decorator** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Decorator(ast_Decorator** ptr);
__attribute__((hot)) ast_Decorator*** core_alloc_alloc_ast_Decorator_ptr(long long count);
__attribute__((hot)) ast_Decorator*** core_alloc_resize_ast_Decorator_ptr(ast_Decorator*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Decorator_ptr(ast_Decorator*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_Decorator** core_alloc_alloc_core_map_MapNode_str_ast_Decorator(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_Decorator(core_map_MapNode_str_ast_Decorator** ptr);

typedef Pattern ast_Pattern;
struct core_vec_Vec_ast_Pattern { ast_Pattern* data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Pattern core_vec_Vec_ast_Pattern;
struct core_vec_Vec_ast_Pattern_ptr { ast_Pattern** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Pattern_ptr core_vec_Vec_ast_Pattern_ptr;
__attribute__((hot)) ast_Pattern* core_alloc_alloc_ast_Pattern(long long count);
__attribute__((hot)) ast_Pattern* core_alloc_resize_ast_Pattern(ast_Pattern* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Pattern(ast_Pattern* ptr);
__attribute__((hot)) ast_Pattern** core_alloc_alloc_ast_Pattern_ptr(long long count);
__attribute__((hot)) ast_Pattern** core_alloc_resize_ast_Pattern_ptr(ast_Pattern** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Pattern_ptr(ast_Pattern** ptr);

typedef Ownership ast_Ownership;
struct core_vec_Vec_ast_Ownership { ast_Ownership* data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Ownership core_vec_Vec_ast_Ownership;
struct core_vec_Vec_ast_Ownership_ptr { ast_Ownership** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Ownership_ptr core_vec_Vec_ast_Ownership_ptr;
__attribute__((hot)) ast_Ownership* core_alloc_alloc_ast_Ownership(long long count);
__attribute__((hot)) ast_Ownership* core_alloc_resize_ast_Ownership(ast_Ownership* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Ownership(ast_Ownership* ptr);
__attribute__((hot)) ast_Ownership** core_alloc_alloc_ast_Ownership_ptr(long long count);
__attribute__((hot)) ast_Ownership** core_alloc_resize_ast_Ownership_ptr(ast_Ownership** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Ownership_ptr(ast_Ownership** ptr);

typedef Comprehension ast_Comprehension;
struct core_vec_Vec_ast_Comprehension { ast_Comprehension** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Comprehension core_vec_Vec_ast_Comprehension;
struct core_vec_Vec_ast_Comprehension_ptr { ast_Comprehension*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Comprehension_ptr core_vec_Vec_ast_Comprehension_ptr;
struct core_map_MapNode_str_ast_Comprehension { char* key; ast_Comprehension* value; struct core_map_MapNode_str_ast_Comprehension* next; };
typedef struct core_map_MapNode_str_ast_Comprehension core_map_MapNode_str_ast_Comprehension;
struct core_map_Map_str_ast_Comprehension { core_map_MapNode_str_ast_Comprehension** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_Comprehension core_map_Map_str_ast_Comprehension;
__attribute__((hot)) ast_Comprehension** core_alloc_alloc_ast_Comprehension(long long count);
__attribute__((hot)) ast_Comprehension** core_alloc_resize_ast_Comprehension(ast_Comprehension** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Comprehension(ast_Comprehension** ptr);
__attribute__((hot)) ast_Comprehension*** core_alloc_alloc_ast_Comprehension_ptr(long long count);
__attribute__((hot)) ast_Comprehension*** core_alloc_resize_ast_Comprehension_ptr(ast_Comprehension*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Comprehension_ptr(ast_Comprehension*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_Comprehension** core_alloc_alloc_core_map_MapNode_str_ast_Comprehension(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_Comprehension(core_map_MapNode_str_ast_Comprehension** ptr);

typedef CatchClause ast_CatchClause;
struct core_vec_Vec_ast_CatchClause { ast_CatchClause** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_CatchClause core_vec_Vec_ast_CatchClause;
struct core_vec_Vec_ast_CatchClause_ptr { ast_CatchClause*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_CatchClause_ptr core_vec_Vec_ast_CatchClause_ptr;
struct core_map_MapNode_str_ast_CatchClause { char* key; ast_CatchClause* value; struct core_map_MapNode_str_ast_CatchClause* next; };
typedef struct core_map_MapNode_str_ast_CatchClause core_map_MapNode_str_ast_CatchClause;
struct core_map_Map_str_ast_CatchClause { core_map_MapNode_str_ast_CatchClause** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_CatchClause core_map_Map_str_ast_CatchClause;
__attribute__((hot)) ast_CatchClause** core_alloc_alloc_ast_CatchClause(long long count);
__attribute__((hot)) ast_CatchClause** core_alloc_resize_ast_CatchClause(ast_CatchClause** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_CatchClause(ast_CatchClause** ptr);
__attribute__((hot)) ast_CatchClause*** core_alloc_alloc_ast_CatchClause_ptr(long long count);
__attribute__((hot)) ast_CatchClause*** core_alloc_resize_ast_CatchClause_ptr(ast_CatchClause*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_CatchClause_ptr(ast_CatchClause*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_CatchClause** core_alloc_alloc_core_map_MapNode_str_ast_CatchClause(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_CatchClause(core_map_MapNode_str_ast_CatchClause** ptr);

typedef MatchArm ast_MatchArm;
struct core_vec_Vec_ast_MatchArm { ast_MatchArm** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_MatchArm core_vec_Vec_ast_MatchArm;
struct core_vec_Vec_ast_MatchArm_ptr { ast_MatchArm*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_MatchArm_ptr core_vec_Vec_ast_MatchArm_ptr;
struct core_map_MapNode_str_ast_MatchArm { char* key; ast_MatchArm* value; struct core_map_MapNode_str_ast_MatchArm* next; };
typedef struct core_map_MapNode_str_ast_MatchArm core_map_MapNode_str_ast_MatchArm;
struct core_map_Map_str_ast_MatchArm { core_map_MapNode_str_ast_MatchArm** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_MatchArm core_map_Map_str_ast_MatchArm;
__attribute__((hot)) ast_MatchArm** core_alloc_alloc_ast_MatchArm(long long count);
__attribute__((hot)) ast_MatchArm** core_alloc_resize_ast_MatchArm(ast_MatchArm** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_MatchArm(ast_MatchArm** ptr);
__attribute__((hot)) ast_MatchArm*** core_alloc_alloc_ast_MatchArm_ptr(long long count);
__attribute__((hot)) ast_MatchArm*** core_alloc_resize_ast_MatchArm_ptr(ast_MatchArm*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_MatchArm_ptr(ast_MatchArm*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_MatchArm** core_alloc_alloc_core_map_MapNode_str_ast_MatchArm(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_MatchArm(core_map_MapNode_str_ast_MatchArm** ptr);

typedef FStringPart ast_FStringPart;
struct core_vec_Vec_ast_FStringPart { ast_FStringPart** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_FStringPart core_vec_Vec_ast_FStringPart;
struct core_vec_Vec_ast_FStringPart_ptr { ast_FStringPart*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_FStringPart_ptr core_vec_Vec_ast_FStringPart_ptr;
struct core_map_MapNode_str_ast_FStringPart { char* key; ast_FStringPart* value; struct core_map_MapNode_str_ast_FStringPart* next; };
typedef struct core_map_MapNode_str_ast_FStringPart core_map_MapNode_str_ast_FStringPart;
struct core_map_Map_str_ast_FStringPart { core_map_MapNode_str_ast_FStringPart** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_FStringPart core_map_Map_str_ast_FStringPart;
__attribute__((hot)) ast_FStringPart** core_alloc_alloc_ast_FStringPart(long long count);
__attribute__((hot)) ast_FStringPart** core_alloc_resize_ast_FStringPart(ast_FStringPart** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_FStringPart(ast_FStringPart** ptr);
__attribute__((hot)) ast_FStringPart*** core_alloc_alloc_ast_FStringPart_ptr(long long count);
__attribute__((hot)) ast_FStringPart*** core_alloc_resize_ast_FStringPart_ptr(ast_FStringPart*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_FStringPart_ptr(ast_FStringPart*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_FStringPart** core_alloc_alloc_core_map_MapNode_str_ast_FStringPart(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_FStringPart(core_map_MapNode_str_ast_FStringPart** ptr);

typedef Expr ast_Expr;
struct core_vec_Vec_ast_Expr { ast_Expr* data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Expr core_vec_Vec_ast_Expr;
struct core_vec_Vec_ast_Expr_ptr { ast_Expr** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Expr_ptr core_vec_Vec_ast_Expr_ptr;
__attribute__((hot)) ast_Expr* core_alloc_alloc_ast_Expr(long long count);
__attribute__((hot)) ast_Expr* core_alloc_resize_ast_Expr(ast_Expr* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Expr(ast_Expr* ptr);
__attribute__((hot)) ast_Expr** core_alloc_alloc_ast_Expr_ptr(long long count);
__attribute__((hot)) ast_Expr** core_alloc_resize_ast_Expr_ptr(ast_Expr** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Expr_ptr(ast_Expr** ptr);

typedef ChanSelectArm ast_ChanSelectArm;
struct core_vec_Vec_ast_ChanSelectArm { ast_ChanSelectArm** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ChanSelectArm core_vec_Vec_ast_ChanSelectArm;
struct core_vec_Vec_ast_ChanSelectArm_ptr { ast_ChanSelectArm*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ChanSelectArm_ptr core_vec_Vec_ast_ChanSelectArm_ptr;
struct core_map_MapNode_str_ast_ChanSelectArm { char* key; ast_ChanSelectArm* value; struct core_map_MapNode_str_ast_ChanSelectArm* next; };
typedef struct core_map_MapNode_str_ast_ChanSelectArm core_map_MapNode_str_ast_ChanSelectArm;
struct core_map_Map_str_ast_ChanSelectArm { core_map_MapNode_str_ast_ChanSelectArm** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_ChanSelectArm core_map_Map_str_ast_ChanSelectArm;
__attribute__((hot)) ast_ChanSelectArm** core_alloc_alloc_ast_ChanSelectArm(long long count);
__attribute__((hot)) ast_ChanSelectArm** core_alloc_resize_ast_ChanSelectArm(ast_ChanSelectArm** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ChanSelectArm(ast_ChanSelectArm** ptr);
__attribute__((hot)) ast_ChanSelectArm*** core_alloc_alloc_ast_ChanSelectArm_ptr(long long count);
__attribute__((hot)) ast_ChanSelectArm*** core_alloc_resize_ast_ChanSelectArm_ptr(ast_ChanSelectArm*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ChanSelectArm_ptr(ast_ChanSelectArm*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_ChanSelectArm** core_alloc_alloc_core_map_MapNode_str_ast_ChanSelectArm(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_ChanSelectArm(core_map_MapNode_str_ast_ChanSelectArm** ptr);

typedef Stmt ast_Stmt;
struct core_vec_Vec_ast_Stmt { ast_Stmt* data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Stmt core_vec_Vec_ast_Stmt;
struct core_vec_Vec_ast_Stmt_ptr { ast_Stmt** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Stmt_ptr core_vec_Vec_ast_Stmt_ptr;
__attribute__((hot)) ast_Stmt* core_alloc_alloc_ast_Stmt(long long count);
__attribute__((hot)) ast_Stmt* core_alloc_resize_ast_Stmt(ast_Stmt* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Stmt(ast_Stmt* ptr);
__attribute__((hot)) ast_Stmt** core_alloc_alloc_ast_Stmt_ptr(long long count);
__attribute__((hot)) ast_Stmt** core_alloc_resize_ast_Stmt_ptr(ast_Stmt** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Stmt_ptr(ast_Stmt** ptr);

typedef Block ast_Block;
struct core_vec_Vec_ast_Block { ast_Block** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Block core_vec_Vec_ast_Block;
struct core_vec_Vec_ast_Block_ptr { ast_Block*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Block_ptr core_vec_Vec_ast_Block_ptr;
struct core_map_MapNode_str_ast_Block { char* key; ast_Block* value; struct core_map_MapNode_str_ast_Block* next; };
typedef struct core_map_MapNode_str_ast_Block core_map_MapNode_str_ast_Block;
struct core_map_Map_str_ast_Block { core_map_MapNode_str_ast_Block** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_Block core_map_Map_str_ast_Block;
__attribute__((hot)) ast_Block** core_alloc_alloc_ast_Block(long long count);
__attribute__((hot)) ast_Block** core_alloc_resize_ast_Block(ast_Block** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Block(ast_Block** ptr);
__attribute__((hot)) ast_Block*** core_alloc_alloc_ast_Block_ptr(long long count);
__attribute__((hot)) ast_Block*** core_alloc_resize_ast_Block_ptr(ast_Block*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Block_ptr(ast_Block*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_Block** core_alloc_alloc_core_map_MapNode_str_ast_Block(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_Block(core_map_MapNode_str_ast_Block** ptr);

typedef ElifClause ast_ElifClause;
struct core_vec_Vec_ast_ElifClause { ast_ElifClause** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ElifClause core_vec_Vec_ast_ElifClause;
struct core_vec_Vec_ast_ElifClause_ptr { ast_ElifClause*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ElifClause_ptr core_vec_Vec_ast_ElifClause_ptr;
struct core_map_MapNode_str_ast_ElifClause { char* key; ast_ElifClause* value; struct core_map_MapNode_str_ast_ElifClause* next; };
typedef struct core_map_MapNode_str_ast_ElifClause core_map_MapNode_str_ast_ElifClause;
struct core_map_Map_str_ast_ElifClause { core_map_MapNode_str_ast_ElifClause** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_ElifClause core_map_Map_str_ast_ElifClause;
__attribute__((hot)) ast_ElifClause** core_alloc_alloc_ast_ElifClause(long long count);
__attribute__((hot)) ast_ElifClause** core_alloc_resize_ast_ElifClause(ast_ElifClause** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ElifClause(ast_ElifClause** ptr);
__attribute__((hot)) ast_ElifClause*** core_alloc_alloc_ast_ElifClause_ptr(long long count);
__attribute__((hot)) ast_ElifClause*** core_alloc_resize_ast_ElifClause_ptr(ast_ElifClause*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ElifClause_ptr(ast_ElifClause*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_ElifClause** core_alloc_alloc_core_map_MapNode_str_ast_ElifClause(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_ElifClause(core_map_MapNode_str_ast_ElifClause** ptr);

typedef Param ast_Param;
struct core_vec_Vec_ast_Param { ast_Param** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Param core_vec_Vec_ast_Param;
struct core_vec_Vec_ast_Param_ptr { ast_Param*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Param_ptr core_vec_Vec_ast_Param_ptr;
struct core_map_MapNode_str_ast_Param { char* key; ast_Param* value; struct core_map_MapNode_str_ast_Param* next; };
typedef struct core_map_MapNode_str_ast_Param core_map_MapNode_str_ast_Param;
struct core_map_Map_str_ast_Param { core_map_MapNode_str_ast_Param** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_Param core_map_Map_str_ast_Param;
__attribute__((hot)) ast_Param** core_alloc_alloc_ast_Param(long long count);
__attribute__((hot)) ast_Param** core_alloc_resize_ast_Param(ast_Param** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Param(ast_Param** ptr);
__attribute__((hot)) ast_Param*** core_alloc_alloc_ast_Param_ptr(long long count);
__attribute__((hot)) ast_Param*** core_alloc_resize_ast_Param_ptr(ast_Param*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Param_ptr(ast_Param*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_Param** core_alloc_alloc_core_map_MapNode_str_ast_Param(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_Param(core_map_MapNode_str_ast_Param** ptr);

typedef FunctionDef ast_FunctionDef;
struct core_vec_Vec_ast_FunctionDef { ast_FunctionDef** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_FunctionDef core_vec_Vec_ast_FunctionDef;
struct core_vec_Vec_ast_FunctionDef_ptr { ast_FunctionDef*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_FunctionDef_ptr core_vec_Vec_ast_FunctionDef_ptr;
struct core_map_MapNode_str_ast_FunctionDef { char* key; ast_FunctionDef* value; struct core_map_MapNode_str_ast_FunctionDef* next; };
typedef struct core_map_MapNode_str_ast_FunctionDef core_map_MapNode_str_ast_FunctionDef;
struct core_map_Map_str_ast_FunctionDef { core_map_MapNode_str_ast_FunctionDef** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_FunctionDef core_map_Map_str_ast_FunctionDef;
__attribute__((hot)) ast_FunctionDef** core_alloc_alloc_ast_FunctionDef(long long count);
__attribute__((hot)) ast_FunctionDef** core_alloc_resize_ast_FunctionDef(ast_FunctionDef** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_FunctionDef(ast_FunctionDef** ptr);
__attribute__((hot)) ast_FunctionDef*** core_alloc_alloc_ast_FunctionDef_ptr(long long count);
__attribute__((hot)) ast_FunctionDef*** core_alloc_resize_ast_FunctionDef_ptr(ast_FunctionDef*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_FunctionDef_ptr(ast_FunctionDef*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_FunctionDef** core_alloc_alloc_core_map_MapNode_str_ast_FunctionDef(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_FunctionDef(core_map_MapNode_str_ast_FunctionDef** ptr);

typedef FieldDef ast_FieldDef;
struct core_vec_Vec_ast_FieldDef { ast_FieldDef** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_FieldDef core_vec_Vec_ast_FieldDef;
struct core_vec_Vec_ast_FieldDef_ptr { ast_FieldDef*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_FieldDef_ptr core_vec_Vec_ast_FieldDef_ptr;
struct core_map_MapNode_str_ast_FieldDef { char* key; ast_FieldDef* value; struct core_map_MapNode_str_ast_FieldDef* next; };
typedef struct core_map_MapNode_str_ast_FieldDef core_map_MapNode_str_ast_FieldDef;
struct core_map_Map_str_ast_FieldDef { core_map_MapNode_str_ast_FieldDef** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_FieldDef core_map_Map_str_ast_FieldDef;
__attribute__((hot)) ast_FieldDef** core_alloc_alloc_ast_FieldDef(long long count);
__attribute__((hot)) ast_FieldDef** core_alloc_resize_ast_FieldDef(ast_FieldDef** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_FieldDef(ast_FieldDef** ptr);
__attribute__((hot)) ast_FieldDef*** core_alloc_alloc_ast_FieldDef_ptr(long long count);
__attribute__((hot)) ast_FieldDef*** core_alloc_resize_ast_FieldDef_ptr(ast_FieldDef*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_FieldDef_ptr(ast_FieldDef*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_FieldDef** core_alloc_alloc_core_map_MapNode_str_ast_FieldDef(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_FieldDef(core_map_MapNode_str_ast_FieldDef** ptr);

typedef ClassDef ast_ClassDef;
struct core_vec_Vec_ast_ClassDef { ast_ClassDef** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ClassDef core_vec_Vec_ast_ClassDef;
struct core_vec_Vec_ast_ClassDef_ptr { ast_ClassDef*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ClassDef_ptr core_vec_Vec_ast_ClassDef_ptr;
struct core_map_MapNode_str_ast_ClassDef { char* key; ast_ClassDef* value; struct core_map_MapNode_str_ast_ClassDef* next; };
typedef struct core_map_MapNode_str_ast_ClassDef core_map_MapNode_str_ast_ClassDef;
struct core_map_Map_str_ast_ClassDef { core_map_MapNode_str_ast_ClassDef** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_ClassDef core_map_Map_str_ast_ClassDef;
__attribute__((hot)) ast_ClassDef** core_alloc_alloc_ast_ClassDef(long long count);
__attribute__((hot)) ast_ClassDef** core_alloc_resize_ast_ClassDef(ast_ClassDef** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ClassDef(ast_ClassDef** ptr);
__attribute__((hot)) ast_ClassDef*** core_alloc_alloc_ast_ClassDef_ptr(long long count);
__attribute__((hot)) ast_ClassDef*** core_alloc_resize_ast_ClassDef_ptr(ast_ClassDef*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ClassDef_ptr(ast_ClassDef*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_ClassDef** core_alloc_alloc_core_map_MapNode_str_ast_ClassDef(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_ClassDef(core_map_MapNode_str_ast_ClassDef** ptr);

typedef VariantDef ast_VariantDef;
struct core_vec_Vec_ast_VariantDef { ast_VariantDef** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_VariantDef core_vec_Vec_ast_VariantDef;
struct core_vec_Vec_ast_VariantDef_ptr { ast_VariantDef*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_VariantDef_ptr core_vec_Vec_ast_VariantDef_ptr;
struct core_map_MapNode_str_ast_VariantDef { char* key; ast_VariantDef* value; struct core_map_MapNode_str_ast_VariantDef* next; };
typedef struct core_map_MapNode_str_ast_VariantDef core_map_MapNode_str_ast_VariantDef;
struct core_map_Map_str_ast_VariantDef { core_map_MapNode_str_ast_VariantDef** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_VariantDef core_map_Map_str_ast_VariantDef;
__attribute__((hot)) ast_VariantDef** core_alloc_alloc_ast_VariantDef(long long count);
__attribute__((hot)) ast_VariantDef** core_alloc_resize_ast_VariantDef(ast_VariantDef** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_VariantDef(ast_VariantDef** ptr);
__attribute__((hot)) ast_VariantDef*** core_alloc_alloc_ast_VariantDef_ptr(long long count);
__attribute__((hot)) ast_VariantDef*** core_alloc_resize_ast_VariantDef_ptr(ast_VariantDef*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_VariantDef_ptr(ast_VariantDef*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_VariantDef** core_alloc_alloc_core_map_MapNode_str_ast_VariantDef(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_VariantDef(core_map_MapNode_str_ast_VariantDef** ptr);

typedef EnumDef ast_EnumDef;
struct core_vec_Vec_ast_EnumDef { ast_EnumDef** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_EnumDef core_vec_Vec_ast_EnumDef;
struct core_vec_Vec_ast_EnumDef_ptr { ast_EnumDef*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_EnumDef_ptr core_vec_Vec_ast_EnumDef_ptr;
struct core_map_MapNode_str_ast_EnumDef { char* key; ast_EnumDef* value; struct core_map_MapNode_str_ast_EnumDef* next; };
typedef struct core_map_MapNode_str_ast_EnumDef core_map_MapNode_str_ast_EnumDef;
struct core_map_Map_str_ast_EnumDef { core_map_MapNode_str_ast_EnumDef** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_EnumDef core_map_Map_str_ast_EnumDef;
__attribute__((hot)) ast_EnumDef** core_alloc_alloc_ast_EnumDef(long long count);
__attribute__((hot)) ast_EnumDef** core_alloc_resize_ast_EnumDef(ast_EnumDef** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_EnumDef(ast_EnumDef** ptr);
__attribute__((hot)) ast_EnumDef*** core_alloc_alloc_ast_EnumDef_ptr(long long count);
__attribute__((hot)) ast_EnumDef*** core_alloc_resize_ast_EnumDef_ptr(ast_EnumDef*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_EnumDef_ptr(ast_EnumDef*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_EnumDef** core_alloc_alloc_core_map_MapNode_str_ast_EnumDef(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_EnumDef(core_map_MapNode_str_ast_EnumDef** ptr);

typedef InterfaceDef ast_InterfaceDef;
struct core_vec_Vec_ast_InterfaceDef { ast_InterfaceDef** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_InterfaceDef core_vec_Vec_ast_InterfaceDef;
struct core_vec_Vec_ast_InterfaceDef_ptr { ast_InterfaceDef*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_InterfaceDef_ptr core_vec_Vec_ast_InterfaceDef_ptr;
struct core_map_MapNode_str_ast_InterfaceDef { char* key; ast_InterfaceDef* value; struct core_map_MapNode_str_ast_InterfaceDef* next; };
typedef struct core_map_MapNode_str_ast_InterfaceDef core_map_MapNode_str_ast_InterfaceDef;
struct core_map_Map_str_ast_InterfaceDef { core_map_MapNode_str_ast_InterfaceDef** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_InterfaceDef core_map_Map_str_ast_InterfaceDef;
__attribute__((hot)) ast_InterfaceDef** core_alloc_alloc_ast_InterfaceDef(long long count);
__attribute__((hot)) ast_InterfaceDef** core_alloc_resize_ast_InterfaceDef(ast_InterfaceDef** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_InterfaceDef(ast_InterfaceDef** ptr);
__attribute__((hot)) ast_InterfaceDef*** core_alloc_alloc_ast_InterfaceDef_ptr(long long count);
__attribute__((hot)) ast_InterfaceDef*** core_alloc_resize_ast_InterfaceDef_ptr(ast_InterfaceDef*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_InterfaceDef_ptr(ast_InterfaceDef*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_InterfaceDef** core_alloc_alloc_core_map_MapNode_str_ast_InterfaceDef(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_InterfaceDef(core_map_MapNode_str_ast_InterfaceDef** ptr);

typedef ImportItem ast_ImportItem;
struct core_vec_Vec_ast_ImportItem { ast_ImportItem** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ImportItem core_vec_Vec_ast_ImportItem;
struct core_vec_Vec_ast_ImportItem_ptr { ast_ImportItem*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_ImportItem_ptr core_vec_Vec_ast_ImportItem_ptr;
struct core_map_MapNode_str_ast_ImportItem { char* key; ast_ImportItem* value; struct core_map_MapNode_str_ast_ImportItem* next; };
typedef struct core_map_MapNode_str_ast_ImportItem core_map_MapNode_str_ast_ImportItem;
struct core_map_Map_str_ast_ImportItem { core_map_MapNode_str_ast_ImportItem** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_ImportItem core_map_Map_str_ast_ImportItem;
__attribute__((hot)) ast_ImportItem** core_alloc_alloc_ast_ImportItem(long long count);
__attribute__((hot)) ast_ImportItem** core_alloc_resize_ast_ImportItem(ast_ImportItem** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ImportItem(ast_ImportItem** ptr);
__attribute__((hot)) ast_ImportItem*** core_alloc_alloc_ast_ImportItem_ptr(long long count);
__attribute__((hot)) ast_ImportItem*** core_alloc_resize_ast_ImportItem_ptr(ast_ImportItem*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_ImportItem_ptr(ast_ImportItem*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_ImportItem** core_alloc_alloc_core_map_MapNode_str_ast_ImportItem(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_ImportItem(core_map_MapNode_str_ast_ImportItem** ptr);

typedef Decl ast_Decl;
struct core_vec_Vec_ast_Decl { ast_Decl* data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Decl core_vec_Vec_ast_Decl;
struct core_vec_Vec_ast_Decl_ptr { ast_Decl** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Decl_ptr core_vec_Vec_ast_Decl_ptr;
__attribute__((hot)) ast_Decl* core_alloc_alloc_ast_Decl(long long count);
__attribute__((hot)) ast_Decl* core_alloc_resize_ast_Decl(ast_Decl* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Decl(ast_Decl* ptr);
__attribute__((hot)) ast_Decl** core_alloc_alloc_ast_Decl_ptr(long long count);
__attribute__((hot)) ast_Decl** core_alloc_resize_ast_Decl_ptr(ast_Decl** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Decl_ptr(ast_Decl** ptr);

typedef Program ast_Program;
struct core_vec_Vec_ast_Program { ast_Program** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Program core_vec_Vec_ast_Program;
struct core_vec_Vec_ast_Program_ptr { ast_Program*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_ast_Program_ptr core_vec_Vec_ast_Program_ptr;
struct core_map_MapNode_str_ast_Program { char* key; ast_Program* value; struct core_map_MapNode_str_ast_Program* next; };
typedef struct core_map_MapNode_str_ast_Program core_map_MapNode_str_ast_Program;
struct core_map_Map_str_ast_Program { core_map_MapNode_str_ast_Program** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_ast_Program core_map_Map_str_ast_Program;
__attribute__((hot)) ast_Program** core_alloc_alloc_ast_Program(long long count);
__attribute__((hot)) ast_Program** core_alloc_resize_ast_Program(ast_Program** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Program(ast_Program** ptr);
__attribute__((hot)) ast_Program*** core_alloc_alloc_ast_Program_ptr(long long count);
__attribute__((hot)) ast_Program*** core_alloc_resize_ast_Program_ptr(ast_Program*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_ast_Program_ptr(ast_Program*** ptr);
__attribute__((hot)) core_map_MapNode_str_ast_Program** core_alloc_alloc_core_map_MapNode_str_ast_Program(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_ast_Program(core_map_MapNode_str_ast_Program** ptr);

typedef Parser parser_Parser;
struct core_vec_Vec_parser_Parser { parser_Parser** data; long long len; long long capacity; };
typedef struct core_vec_Vec_parser_Parser core_vec_Vec_parser_Parser;
struct core_vec_Vec_parser_Parser_ptr { parser_Parser*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_parser_Parser_ptr core_vec_Vec_parser_Parser_ptr;
struct core_map_MapNode_str_parser_Parser { char* key; parser_Parser* value; struct core_map_MapNode_str_parser_Parser* next; };
typedef struct core_map_MapNode_str_parser_Parser core_map_MapNode_str_parser_Parser;
struct core_map_Map_str_parser_Parser { core_map_MapNode_str_parser_Parser** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_parser_Parser core_map_Map_str_parser_Parser;
__attribute__((hot)) parser_Parser** core_alloc_alloc_parser_Parser(long long count);
__attribute__((hot)) parser_Parser** core_alloc_resize_parser_Parser(parser_Parser** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_parser_Parser(parser_Parser** ptr);
__attribute__((hot)) parser_Parser*** core_alloc_alloc_parser_Parser_ptr(long long count);
__attribute__((hot)) parser_Parser*** core_alloc_resize_parser_Parser_ptr(parser_Parser*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_parser_Parser_ptr(parser_Parser*** ptr);
__attribute__((hot)) core_map_MapNode_str_parser_Parser** core_alloc_alloc_core_map_MapNode_str_parser_Parser(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_parser_Parser(core_map_MapNode_str_parser_Parser** ptr);

typedef ModuleResolver resolver_ModuleResolver;
struct core_vec_Vec_resolver_ModuleResolver { resolver_ModuleResolver** data; long long len; long long capacity; };
typedef struct core_vec_Vec_resolver_ModuleResolver core_vec_Vec_resolver_ModuleResolver;
struct core_vec_Vec_resolver_ModuleResolver_ptr { resolver_ModuleResolver*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_resolver_ModuleResolver_ptr core_vec_Vec_resolver_ModuleResolver_ptr;
struct core_map_MapNode_str_resolver_ModuleResolver { char* key; resolver_ModuleResolver* value; struct core_map_MapNode_str_resolver_ModuleResolver* next; };
typedef struct core_map_MapNode_str_resolver_ModuleResolver core_map_MapNode_str_resolver_ModuleResolver;
struct core_map_Map_str_resolver_ModuleResolver { core_map_MapNode_str_resolver_ModuleResolver** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_resolver_ModuleResolver core_map_Map_str_resolver_ModuleResolver;
__attribute__((hot)) resolver_ModuleResolver** core_alloc_alloc_resolver_ModuleResolver(long long count);
__attribute__((hot)) resolver_ModuleResolver** core_alloc_resize_resolver_ModuleResolver(resolver_ModuleResolver** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_resolver_ModuleResolver(resolver_ModuleResolver** ptr);
__attribute__((hot)) resolver_ModuleResolver*** core_alloc_alloc_resolver_ModuleResolver_ptr(long long count);
__attribute__((hot)) resolver_ModuleResolver*** core_alloc_resize_resolver_ModuleResolver_ptr(resolver_ModuleResolver*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_resolver_ModuleResolver_ptr(resolver_ModuleResolver*** ptr);
__attribute__((hot)) core_map_MapNode_str_resolver_ModuleResolver** core_alloc_alloc_core_map_MapNode_str_resolver_ModuleResolver(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_resolver_ModuleResolver(core_map_MapNode_str_resolver_ModuleResolver** ptr);

typedef HirExpr hir_HirExpr;
struct core_vec_Vec_hir_HirExpr { hir_HirExpr* data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirExpr core_vec_Vec_hir_HirExpr;
struct core_vec_Vec_hir_HirExpr_ptr { hir_HirExpr** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirExpr_ptr core_vec_Vec_hir_HirExpr_ptr;
__attribute__((hot)) hir_HirExpr* core_alloc_alloc_hir_HirExpr(long long count);
__attribute__((hot)) hir_HirExpr* core_alloc_resize_hir_HirExpr(hir_HirExpr* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirExpr(hir_HirExpr* ptr);
__attribute__((hot)) hir_HirExpr** core_alloc_alloc_hir_HirExpr_ptr(long long count);
__attribute__((hot)) hir_HirExpr** core_alloc_resize_hir_HirExpr_ptr(hir_HirExpr** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirExpr_ptr(hir_HirExpr** ptr);

typedef HirComprehension hir_HirComprehension;
struct core_vec_Vec_hir_HirComprehension { hir_HirComprehension** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirComprehension core_vec_Vec_hir_HirComprehension;
struct core_vec_Vec_hir_HirComprehension_ptr { hir_HirComprehension*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirComprehension_ptr core_vec_Vec_hir_HirComprehension_ptr;
struct core_map_MapNode_str_hir_HirComprehension { char* key; hir_HirComprehension* value; struct core_map_MapNode_str_hir_HirComprehension* next; };
typedef struct core_map_MapNode_str_hir_HirComprehension core_map_MapNode_str_hir_HirComprehension;
struct core_map_Map_str_hir_HirComprehension { core_map_MapNode_str_hir_HirComprehension** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirComprehension core_map_Map_str_hir_HirComprehension;
__attribute__((hot)) hir_HirComprehension** core_alloc_alloc_hir_HirComprehension(long long count);
__attribute__((hot)) hir_HirComprehension** core_alloc_resize_hir_HirComprehension(hir_HirComprehension** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirComprehension(hir_HirComprehension** ptr);
__attribute__((hot)) hir_HirComprehension*** core_alloc_alloc_hir_HirComprehension_ptr(long long count);
__attribute__((hot)) hir_HirComprehension*** core_alloc_resize_hir_HirComprehension_ptr(hir_HirComprehension*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirComprehension_ptr(hir_HirComprehension*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirComprehension** core_alloc_alloc_core_map_MapNode_str_hir_HirComprehension(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirComprehension(core_map_MapNode_str_hir_HirComprehension** ptr);

typedef HirCatchClause hir_HirCatchClause;
struct core_vec_Vec_hir_HirCatchClause { hir_HirCatchClause** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirCatchClause core_vec_Vec_hir_HirCatchClause;
struct core_vec_Vec_hir_HirCatchClause_ptr { hir_HirCatchClause*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirCatchClause_ptr core_vec_Vec_hir_HirCatchClause_ptr;
struct core_map_MapNode_str_hir_HirCatchClause { char* key; hir_HirCatchClause* value; struct core_map_MapNode_str_hir_HirCatchClause* next; };
typedef struct core_map_MapNode_str_hir_HirCatchClause core_map_MapNode_str_hir_HirCatchClause;
struct core_map_Map_str_hir_HirCatchClause { core_map_MapNode_str_hir_HirCatchClause** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirCatchClause core_map_Map_str_hir_HirCatchClause;
__attribute__((hot)) hir_HirCatchClause** core_alloc_alloc_hir_HirCatchClause(long long count);
__attribute__((hot)) hir_HirCatchClause** core_alloc_resize_hir_HirCatchClause(hir_HirCatchClause** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirCatchClause(hir_HirCatchClause** ptr);
__attribute__((hot)) hir_HirCatchClause*** core_alloc_alloc_hir_HirCatchClause_ptr(long long count);
__attribute__((hot)) hir_HirCatchClause*** core_alloc_resize_hir_HirCatchClause_ptr(hir_HirCatchClause*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirCatchClause_ptr(hir_HirCatchClause*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirCatchClause** core_alloc_alloc_core_map_MapNode_str_hir_HirCatchClause(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirCatchClause(core_map_MapNode_str_hir_HirCatchClause** ptr);

typedef HirFStringPart hir_HirFStringPart;
struct core_vec_Vec_hir_HirFStringPart { hir_HirFStringPart** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirFStringPart core_vec_Vec_hir_HirFStringPart;
struct core_vec_Vec_hir_HirFStringPart_ptr { hir_HirFStringPart*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirFStringPart_ptr core_vec_Vec_hir_HirFStringPart_ptr;
struct core_map_MapNode_str_hir_HirFStringPart { char* key; hir_HirFStringPart* value; struct core_map_MapNode_str_hir_HirFStringPart* next; };
typedef struct core_map_MapNode_str_hir_HirFStringPart core_map_MapNode_str_hir_HirFStringPart;
struct core_map_Map_str_hir_HirFStringPart { core_map_MapNode_str_hir_HirFStringPart** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirFStringPart core_map_Map_str_hir_HirFStringPart;
__attribute__((hot)) hir_HirFStringPart** core_alloc_alloc_hir_HirFStringPart(long long count);
__attribute__((hot)) hir_HirFStringPart** core_alloc_resize_hir_HirFStringPart(hir_HirFStringPart** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirFStringPart(hir_HirFStringPart** ptr);
__attribute__((hot)) hir_HirFStringPart*** core_alloc_alloc_hir_HirFStringPart_ptr(long long count);
__attribute__((hot)) hir_HirFStringPart*** core_alloc_resize_hir_HirFStringPart_ptr(hir_HirFStringPart*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirFStringPart_ptr(hir_HirFStringPart*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirFStringPart** core_alloc_alloc_core_map_MapNode_str_hir_HirFStringPart(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirFStringPart(core_map_MapNode_str_hir_HirFStringPart** ptr);

typedef HirMatchArm hir_HirMatchArm;
struct core_vec_Vec_hir_HirMatchArm { hir_HirMatchArm** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirMatchArm core_vec_Vec_hir_HirMatchArm;
struct core_vec_Vec_hir_HirMatchArm_ptr { hir_HirMatchArm*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirMatchArm_ptr core_vec_Vec_hir_HirMatchArm_ptr;
struct core_map_MapNode_str_hir_HirMatchArm { char* key; hir_HirMatchArm* value; struct core_map_MapNode_str_hir_HirMatchArm* next; };
typedef struct core_map_MapNode_str_hir_HirMatchArm core_map_MapNode_str_hir_HirMatchArm;
struct core_map_Map_str_hir_HirMatchArm { core_map_MapNode_str_hir_HirMatchArm** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirMatchArm core_map_Map_str_hir_HirMatchArm;
__attribute__((hot)) hir_HirMatchArm** core_alloc_alloc_hir_HirMatchArm(long long count);
__attribute__((hot)) hir_HirMatchArm** core_alloc_resize_hir_HirMatchArm(hir_HirMatchArm** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirMatchArm(hir_HirMatchArm** ptr);
__attribute__((hot)) hir_HirMatchArm*** core_alloc_alloc_hir_HirMatchArm_ptr(long long count);
__attribute__((hot)) hir_HirMatchArm*** core_alloc_resize_hir_HirMatchArm_ptr(hir_HirMatchArm*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirMatchArm_ptr(hir_HirMatchArm*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirMatchArm** core_alloc_alloc_core_map_MapNode_str_hir_HirMatchArm(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirMatchArm(core_map_MapNode_str_hir_HirMatchArm** ptr);

typedef HirChanSelectArm hir_HirChanSelectArm;
struct core_vec_Vec_hir_HirChanSelectArm { hir_HirChanSelectArm** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirChanSelectArm core_vec_Vec_hir_HirChanSelectArm;
struct core_vec_Vec_hir_HirChanSelectArm_ptr { hir_HirChanSelectArm*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirChanSelectArm_ptr core_vec_Vec_hir_HirChanSelectArm_ptr;
struct core_map_MapNode_str_hir_HirChanSelectArm { char* key; hir_HirChanSelectArm* value; struct core_map_MapNode_str_hir_HirChanSelectArm* next; };
typedef struct core_map_MapNode_str_hir_HirChanSelectArm core_map_MapNode_str_hir_HirChanSelectArm;
struct core_map_Map_str_hir_HirChanSelectArm { core_map_MapNode_str_hir_HirChanSelectArm** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirChanSelectArm core_map_Map_str_hir_HirChanSelectArm;
__attribute__((hot)) hir_HirChanSelectArm** core_alloc_alloc_hir_HirChanSelectArm(long long count);
__attribute__((hot)) hir_HirChanSelectArm** core_alloc_resize_hir_HirChanSelectArm(hir_HirChanSelectArm** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirChanSelectArm(hir_HirChanSelectArm** ptr);
__attribute__((hot)) hir_HirChanSelectArm*** core_alloc_alloc_hir_HirChanSelectArm_ptr(long long count);
__attribute__((hot)) hir_HirChanSelectArm*** core_alloc_resize_hir_HirChanSelectArm_ptr(hir_HirChanSelectArm*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirChanSelectArm_ptr(hir_HirChanSelectArm*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirChanSelectArm** core_alloc_alloc_core_map_MapNode_str_hir_HirChanSelectArm(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirChanSelectArm(core_map_MapNode_str_hir_HirChanSelectArm** ptr);

typedef HirStmt hir_HirStmt;
struct core_vec_Vec_hir_HirStmt { hir_HirStmt* data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirStmt core_vec_Vec_hir_HirStmt;
struct core_vec_Vec_hir_HirStmt_ptr { hir_HirStmt** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirStmt_ptr core_vec_Vec_hir_HirStmt_ptr;
__attribute__((hot)) hir_HirStmt* core_alloc_alloc_hir_HirStmt(long long count);
__attribute__((hot)) hir_HirStmt* core_alloc_resize_hir_HirStmt(hir_HirStmt* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirStmt(hir_HirStmt* ptr);
__attribute__((hot)) hir_HirStmt** core_alloc_alloc_hir_HirStmt_ptr(long long count);
__attribute__((hot)) hir_HirStmt** core_alloc_resize_hir_HirStmt_ptr(hir_HirStmt** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirStmt_ptr(hir_HirStmt** ptr);

typedef HirBlock hir_HirBlock;
struct core_vec_Vec_hir_HirBlock { hir_HirBlock** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirBlock core_vec_Vec_hir_HirBlock;
struct core_vec_Vec_hir_HirBlock_ptr { hir_HirBlock*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirBlock_ptr core_vec_Vec_hir_HirBlock_ptr;
struct core_map_MapNode_str_hir_HirBlock { char* key; hir_HirBlock* value; struct core_map_MapNode_str_hir_HirBlock* next; };
typedef struct core_map_MapNode_str_hir_HirBlock core_map_MapNode_str_hir_HirBlock;
struct core_map_Map_str_hir_HirBlock { core_map_MapNode_str_hir_HirBlock** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirBlock core_map_Map_str_hir_HirBlock;
__attribute__((hot)) hir_HirBlock** core_alloc_alloc_hir_HirBlock(long long count);
__attribute__((hot)) hir_HirBlock** core_alloc_resize_hir_HirBlock(hir_HirBlock** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirBlock(hir_HirBlock** ptr);
__attribute__((hot)) hir_HirBlock*** core_alloc_alloc_hir_HirBlock_ptr(long long count);
__attribute__((hot)) hir_HirBlock*** core_alloc_resize_hir_HirBlock_ptr(hir_HirBlock*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirBlock_ptr(hir_HirBlock*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirBlock** core_alloc_alloc_core_map_MapNode_str_hir_HirBlock(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirBlock(core_map_MapNode_str_hir_HirBlock** ptr);

typedef HirParam hir_HirParam;
struct core_vec_Vec_hir_HirParam { hir_HirParam** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirParam core_vec_Vec_hir_HirParam;
struct core_vec_Vec_hir_HirParam_ptr { hir_HirParam*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirParam_ptr core_vec_Vec_hir_HirParam_ptr;
struct core_map_MapNode_str_hir_HirParam { char* key; hir_HirParam* value; struct core_map_MapNode_str_hir_HirParam* next; };
typedef struct core_map_MapNode_str_hir_HirParam core_map_MapNode_str_hir_HirParam;
struct core_map_Map_str_hir_HirParam { core_map_MapNode_str_hir_HirParam** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirParam core_map_Map_str_hir_HirParam;
__attribute__((hot)) hir_HirParam** core_alloc_alloc_hir_HirParam(long long count);
__attribute__((hot)) hir_HirParam** core_alloc_resize_hir_HirParam(hir_HirParam** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirParam(hir_HirParam** ptr);
__attribute__((hot)) hir_HirParam*** core_alloc_alloc_hir_HirParam_ptr(long long count);
__attribute__((hot)) hir_HirParam*** core_alloc_resize_hir_HirParam_ptr(hir_HirParam*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirParam_ptr(hir_HirParam*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirParam** core_alloc_alloc_core_map_MapNode_str_hir_HirParam(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirParam(core_map_MapNode_str_hir_HirParam** ptr);

typedef HirFunction hir_HirFunction;
struct core_vec_Vec_hir_HirFunction { hir_HirFunction** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirFunction core_vec_Vec_hir_HirFunction;
struct core_vec_Vec_hir_HirFunction_ptr { hir_HirFunction*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirFunction_ptr core_vec_Vec_hir_HirFunction_ptr;
struct core_map_MapNode_str_hir_HirFunction { char* key; hir_HirFunction* value; struct core_map_MapNode_str_hir_HirFunction* next; };
typedef struct core_map_MapNode_str_hir_HirFunction core_map_MapNode_str_hir_HirFunction;
struct core_map_Map_str_hir_HirFunction { core_map_MapNode_str_hir_HirFunction** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirFunction core_map_Map_str_hir_HirFunction;
__attribute__((hot)) hir_HirFunction** core_alloc_alloc_hir_HirFunction(long long count);
__attribute__((hot)) hir_HirFunction** core_alloc_resize_hir_HirFunction(hir_HirFunction** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirFunction(hir_HirFunction** ptr);
__attribute__((hot)) hir_HirFunction*** core_alloc_alloc_hir_HirFunction_ptr(long long count);
__attribute__((hot)) hir_HirFunction*** core_alloc_resize_hir_HirFunction_ptr(hir_HirFunction*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirFunction_ptr(hir_HirFunction*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirFunction** core_alloc_alloc_core_map_MapNode_str_hir_HirFunction(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirFunction(core_map_MapNode_str_hir_HirFunction** ptr);

typedef HirField hir_HirField;
struct core_vec_Vec_hir_HirField { hir_HirField** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirField core_vec_Vec_hir_HirField;
struct core_vec_Vec_hir_HirField_ptr { hir_HirField*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirField_ptr core_vec_Vec_hir_HirField_ptr;
struct core_map_MapNode_str_hir_HirField { char* key; hir_HirField* value; struct core_map_MapNode_str_hir_HirField* next; };
typedef struct core_map_MapNode_str_hir_HirField core_map_MapNode_str_hir_HirField;
struct core_map_Map_str_hir_HirField { core_map_MapNode_str_hir_HirField** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirField core_map_Map_str_hir_HirField;
__attribute__((hot)) hir_HirField** core_alloc_alloc_hir_HirField(long long count);
__attribute__((hot)) hir_HirField** core_alloc_resize_hir_HirField(hir_HirField** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirField(hir_HirField** ptr);
__attribute__((hot)) hir_HirField*** core_alloc_alloc_hir_HirField_ptr(long long count);
__attribute__((hot)) hir_HirField*** core_alloc_resize_hir_HirField_ptr(hir_HirField*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirField_ptr(hir_HirField*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirField** core_alloc_alloc_core_map_MapNode_str_hir_HirField(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirField(core_map_MapNode_str_hir_HirField** ptr);

typedef HirClass hir_HirClass;
struct core_vec_Vec_hir_HirClass { hir_HirClass** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirClass core_vec_Vec_hir_HirClass;
struct core_vec_Vec_hir_HirClass_ptr { hir_HirClass*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirClass_ptr core_vec_Vec_hir_HirClass_ptr;
struct core_map_MapNode_str_hir_HirClass { char* key; hir_HirClass* value; struct core_map_MapNode_str_hir_HirClass* next; };
typedef struct core_map_MapNode_str_hir_HirClass core_map_MapNode_str_hir_HirClass;
struct core_map_Map_str_hir_HirClass { core_map_MapNode_str_hir_HirClass** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirClass core_map_Map_str_hir_HirClass;
__attribute__((hot)) hir_HirClass** core_alloc_alloc_hir_HirClass(long long count);
__attribute__((hot)) hir_HirClass** core_alloc_resize_hir_HirClass(hir_HirClass** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirClass(hir_HirClass** ptr);
__attribute__((hot)) hir_HirClass*** core_alloc_alloc_hir_HirClass_ptr(long long count);
__attribute__((hot)) hir_HirClass*** core_alloc_resize_hir_HirClass_ptr(hir_HirClass*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirClass_ptr(hir_HirClass*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirClass** core_alloc_alloc_core_map_MapNode_str_hir_HirClass(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirClass(core_map_MapNode_str_hir_HirClass** ptr);

typedef HirVariant hir_HirVariant;
struct core_vec_Vec_hir_HirVariant { hir_HirVariant** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirVariant core_vec_Vec_hir_HirVariant;
struct core_vec_Vec_hir_HirVariant_ptr { hir_HirVariant*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirVariant_ptr core_vec_Vec_hir_HirVariant_ptr;
struct core_map_MapNode_str_hir_HirVariant { char* key; hir_HirVariant* value; struct core_map_MapNode_str_hir_HirVariant* next; };
typedef struct core_map_MapNode_str_hir_HirVariant core_map_MapNode_str_hir_HirVariant;
struct core_map_Map_str_hir_HirVariant { core_map_MapNode_str_hir_HirVariant** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirVariant core_map_Map_str_hir_HirVariant;
__attribute__((hot)) hir_HirVariant** core_alloc_alloc_hir_HirVariant(long long count);
__attribute__((hot)) hir_HirVariant** core_alloc_resize_hir_HirVariant(hir_HirVariant** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirVariant(hir_HirVariant** ptr);
__attribute__((hot)) hir_HirVariant*** core_alloc_alloc_hir_HirVariant_ptr(long long count);
__attribute__((hot)) hir_HirVariant*** core_alloc_resize_hir_HirVariant_ptr(hir_HirVariant*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirVariant_ptr(hir_HirVariant*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirVariant** core_alloc_alloc_core_map_MapNode_str_hir_HirVariant(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirVariant(core_map_MapNode_str_hir_HirVariant** ptr);

typedef HirEnum hir_HirEnum;
struct core_vec_Vec_hir_HirEnum { hir_HirEnum** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirEnum core_vec_Vec_hir_HirEnum;
struct core_vec_Vec_hir_HirEnum_ptr { hir_HirEnum*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirEnum_ptr core_vec_Vec_hir_HirEnum_ptr;
struct core_map_MapNode_str_hir_HirEnum { char* key; hir_HirEnum* value; struct core_map_MapNode_str_hir_HirEnum* next; };
typedef struct core_map_MapNode_str_hir_HirEnum core_map_MapNode_str_hir_HirEnum;
struct core_map_Map_str_hir_HirEnum { core_map_MapNode_str_hir_HirEnum** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirEnum core_map_Map_str_hir_HirEnum;
__attribute__((hot)) hir_HirEnum** core_alloc_alloc_hir_HirEnum(long long count);
__attribute__((hot)) hir_HirEnum** core_alloc_resize_hir_HirEnum(hir_HirEnum** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirEnum(hir_HirEnum** ptr);
__attribute__((hot)) hir_HirEnum*** core_alloc_alloc_hir_HirEnum_ptr(long long count);
__attribute__((hot)) hir_HirEnum*** core_alloc_resize_hir_HirEnum_ptr(hir_HirEnum*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirEnum_ptr(hir_HirEnum*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirEnum** core_alloc_alloc_core_map_MapNode_str_hir_HirEnum(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirEnum(core_map_MapNode_str_hir_HirEnum** ptr);

typedef HirInterface hir_HirInterface;
struct core_vec_Vec_hir_HirInterface { hir_HirInterface** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirInterface core_vec_Vec_hir_HirInterface;
struct core_vec_Vec_hir_HirInterface_ptr { hir_HirInterface*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirInterface_ptr core_vec_Vec_hir_HirInterface_ptr;
struct core_map_MapNode_str_hir_HirInterface { char* key; hir_HirInterface* value; struct core_map_MapNode_str_hir_HirInterface* next; };
typedef struct core_map_MapNode_str_hir_HirInterface core_map_MapNode_str_hir_HirInterface;
struct core_map_Map_str_hir_HirInterface { core_map_MapNode_str_hir_HirInterface** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirInterface core_map_Map_str_hir_HirInterface;
__attribute__((hot)) hir_HirInterface** core_alloc_alloc_hir_HirInterface(long long count);
__attribute__((hot)) hir_HirInterface** core_alloc_resize_hir_HirInterface(hir_HirInterface** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirInterface(hir_HirInterface** ptr);
__attribute__((hot)) hir_HirInterface*** core_alloc_alloc_hir_HirInterface_ptr(long long count);
__attribute__((hot)) hir_HirInterface*** core_alloc_resize_hir_HirInterface_ptr(hir_HirInterface*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirInterface_ptr(hir_HirInterface*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirInterface** core_alloc_alloc_core_map_MapNode_str_hir_HirInterface(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirInterface(core_map_MapNode_str_hir_HirInterface** ptr);

typedef HirProgram hir_HirProgram;
struct core_vec_Vec_hir_HirProgram { hir_HirProgram** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirProgram core_vec_Vec_hir_HirProgram;
struct core_vec_Vec_hir_HirProgram_ptr { hir_HirProgram*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_hir_HirProgram_ptr core_vec_Vec_hir_HirProgram_ptr;
struct core_map_MapNode_str_hir_HirProgram { char* key; hir_HirProgram* value; struct core_map_MapNode_str_hir_HirProgram* next; };
typedef struct core_map_MapNode_str_hir_HirProgram core_map_MapNode_str_hir_HirProgram;
struct core_map_Map_str_hir_HirProgram { core_map_MapNode_str_hir_HirProgram** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_hir_HirProgram core_map_Map_str_hir_HirProgram;
__attribute__((hot)) hir_HirProgram** core_alloc_alloc_hir_HirProgram(long long count);
__attribute__((hot)) hir_HirProgram** core_alloc_resize_hir_HirProgram(hir_HirProgram** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirProgram(hir_HirProgram** ptr);
__attribute__((hot)) hir_HirProgram*** core_alloc_alloc_hir_HirProgram_ptr(long long count);
__attribute__((hot)) hir_HirProgram*** core_alloc_resize_hir_HirProgram_ptr(hir_HirProgram*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_hir_HirProgram_ptr(hir_HirProgram*** ptr);
__attribute__((hot)) core_map_MapNode_str_hir_HirProgram** core_alloc_alloc_core_map_MapNode_str_hir_HirProgram(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_hir_HirProgram(core_map_MapNode_str_hir_HirProgram** ptr);

typedef MirStmt mir_MirStmt;
struct core_vec_Vec_mir_MirStmt { mir_MirStmt* data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_MirStmt core_vec_Vec_mir_MirStmt;
struct core_vec_Vec_mir_MirStmt_ptr { mir_MirStmt** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_MirStmt_ptr core_vec_Vec_mir_MirStmt_ptr;
__attribute__((hot)) mir_MirStmt* core_alloc_alloc_mir_MirStmt(long long count);
__attribute__((hot)) mir_MirStmt* core_alloc_resize_mir_MirStmt(mir_MirStmt* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_MirStmt(mir_MirStmt* ptr);
__attribute__((hot)) mir_MirStmt** core_alloc_alloc_mir_MirStmt_ptr(long long count);
__attribute__((hot)) mir_MirStmt** core_alloc_resize_mir_MirStmt_ptr(mir_MirStmt** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_MirStmt_ptr(mir_MirStmt** ptr);

typedef MirTerm mir_MirTerm;
struct core_vec_Vec_mir_MirTerm { mir_MirTerm* data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_MirTerm core_vec_Vec_mir_MirTerm;
struct core_vec_Vec_mir_MirTerm_ptr { mir_MirTerm** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_MirTerm_ptr core_vec_Vec_mir_MirTerm_ptr;
__attribute__((hot)) mir_MirTerm* core_alloc_alloc_mir_MirTerm(long long count);
__attribute__((hot)) mir_MirTerm* core_alloc_resize_mir_MirTerm(mir_MirTerm* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_MirTerm(mir_MirTerm* ptr);
__attribute__((hot)) mir_MirTerm** core_alloc_alloc_mir_MirTerm_ptr(long long count);
__attribute__((hot)) mir_MirTerm** core_alloc_resize_mir_MirTerm_ptr(mir_MirTerm** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_MirTerm_ptr(mir_MirTerm** ptr);

typedef MirBlock mir_MirBlock;
struct core_vec_Vec_mir_MirBlock { mir_MirBlock** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_MirBlock core_vec_Vec_mir_MirBlock;
struct core_vec_Vec_mir_MirBlock_ptr { mir_MirBlock*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_MirBlock_ptr core_vec_Vec_mir_MirBlock_ptr;
struct core_map_MapNode_str_mir_MirBlock { char* key; mir_MirBlock* value; struct core_map_MapNode_str_mir_MirBlock* next; };
typedef struct core_map_MapNode_str_mir_MirBlock core_map_MapNode_str_mir_MirBlock;
struct core_map_Map_str_mir_MirBlock { core_map_MapNode_str_mir_MirBlock** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_mir_MirBlock core_map_Map_str_mir_MirBlock;
__attribute__((hot)) mir_MirBlock** core_alloc_alloc_mir_MirBlock(long long count);
__attribute__((hot)) mir_MirBlock** core_alloc_resize_mir_MirBlock(mir_MirBlock** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_MirBlock(mir_MirBlock** ptr);
__attribute__((hot)) mir_MirBlock*** core_alloc_alloc_mir_MirBlock_ptr(long long count);
__attribute__((hot)) mir_MirBlock*** core_alloc_resize_mir_MirBlock_ptr(mir_MirBlock*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_MirBlock_ptr(mir_MirBlock*** ptr);
__attribute__((hot)) core_map_MapNode_str_mir_MirBlock** core_alloc_alloc_core_map_MapNode_str_mir_MirBlock(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_mir_MirBlock(core_map_MapNode_str_mir_MirBlock** ptr);

typedef DropSite mir_DropSite;
struct core_vec_Vec_mir_DropSite { mir_DropSite** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_DropSite core_vec_Vec_mir_DropSite;
struct core_vec_Vec_mir_DropSite_ptr { mir_DropSite*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_DropSite_ptr core_vec_Vec_mir_DropSite_ptr;
struct core_map_MapNode_str_mir_DropSite { char* key; mir_DropSite* value; struct core_map_MapNode_str_mir_DropSite* next; };
typedef struct core_map_MapNode_str_mir_DropSite core_map_MapNode_str_mir_DropSite;
struct core_map_Map_str_mir_DropSite { core_map_MapNode_str_mir_DropSite** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_mir_DropSite core_map_Map_str_mir_DropSite;
__attribute__((hot)) mir_DropSite** core_alloc_alloc_mir_DropSite(long long count);
__attribute__((hot)) mir_DropSite** core_alloc_resize_mir_DropSite(mir_DropSite** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_DropSite(mir_DropSite** ptr);
__attribute__((hot)) mir_DropSite*** core_alloc_alloc_mir_DropSite_ptr(long long count);
__attribute__((hot)) mir_DropSite*** core_alloc_resize_mir_DropSite_ptr(mir_DropSite*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_DropSite_ptr(mir_DropSite*** ptr);
__attribute__((hot)) core_map_MapNode_str_mir_DropSite** core_alloc_alloc_core_map_MapNode_str_mir_DropSite(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_mir_DropSite(core_map_MapNode_str_mir_DropSite** ptr);

typedef BorrowEdge mir_BorrowEdge;
struct core_vec_Vec_mir_BorrowEdge { mir_BorrowEdge** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_BorrowEdge core_vec_Vec_mir_BorrowEdge;
struct core_vec_Vec_mir_BorrowEdge_ptr { mir_BorrowEdge*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_BorrowEdge_ptr core_vec_Vec_mir_BorrowEdge_ptr;
struct core_map_MapNode_str_mir_BorrowEdge { char* key; mir_BorrowEdge* value; struct core_map_MapNode_str_mir_BorrowEdge* next; };
typedef struct core_map_MapNode_str_mir_BorrowEdge core_map_MapNode_str_mir_BorrowEdge;
struct core_map_Map_str_mir_BorrowEdge { core_map_MapNode_str_mir_BorrowEdge** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_mir_BorrowEdge core_map_Map_str_mir_BorrowEdge;
__attribute__((hot)) mir_BorrowEdge** core_alloc_alloc_mir_BorrowEdge(long long count);
__attribute__((hot)) mir_BorrowEdge** core_alloc_resize_mir_BorrowEdge(mir_BorrowEdge** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_BorrowEdge(mir_BorrowEdge** ptr);
__attribute__((hot)) mir_BorrowEdge*** core_alloc_alloc_mir_BorrowEdge_ptr(long long count);
__attribute__((hot)) mir_BorrowEdge*** core_alloc_resize_mir_BorrowEdge_ptr(mir_BorrowEdge*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_BorrowEdge_ptr(mir_BorrowEdge*** ptr);
__attribute__((hot)) core_map_MapNode_str_mir_BorrowEdge** core_alloc_alloc_core_map_MapNode_str_mir_BorrowEdge(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_mir_BorrowEdge(core_map_MapNode_str_mir_BorrowEdge** ptr);

typedef MirFunction mir_MirFunction;
struct core_vec_Vec_mir_MirFunction { mir_MirFunction** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_MirFunction core_vec_Vec_mir_MirFunction;
struct core_vec_Vec_mir_MirFunction_ptr { mir_MirFunction*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_MirFunction_ptr core_vec_Vec_mir_MirFunction_ptr;
struct core_map_MapNode_str_mir_MirFunction { char* key; mir_MirFunction* value; struct core_map_MapNode_str_mir_MirFunction* next; };
typedef struct core_map_MapNode_str_mir_MirFunction core_map_MapNode_str_mir_MirFunction;
struct core_map_Map_str_mir_MirFunction { core_map_MapNode_str_mir_MirFunction** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_mir_MirFunction core_map_Map_str_mir_MirFunction;
__attribute__((hot)) mir_MirFunction** core_alloc_alloc_mir_MirFunction(long long count);
__attribute__((hot)) mir_MirFunction** core_alloc_resize_mir_MirFunction(mir_MirFunction** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_MirFunction(mir_MirFunction** ptr);
__attribute__((hot)) mir_MirFunction*** core_alloc_alloc_mir_MirFunction_ptr(long long count);
__attribute__((hot)) mir_MirFunction*** core_alloc_resize_mir_MirFunction_ptr(mir_MirFunction*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_MirFunction_ptr(mir_MirFunction*** ptr);
__attribute__((hot)) core_map_MapNode_str_mir_MirFunction** core_alloc_alloc_core_map_MapNode_str_mir_MirFunction(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_mir_MirFunction(core_map_MapNode_str_mir_MirFunction** ptr);

typedef MirProgram mir_MirProgram;
struct core_vec_Vec_mir_MirProgram { mir_MirProgram** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_MirProgram core_vec_Vec_mir_MirProgram;
struct core_vec_Vec_mir_MirProgram_ptr { mir_MirProgram*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_MirProgram_ptr core_vec_Vec_mir_MirProgram_ptr;
struct core_map_MapNode_str_mir_MirProgram { char* key; mir_MirProgram* value; struct core_map_MapNode_str_mir_MirProgram* next; };
typedef struct core_map_MapNode_str_mir_MirProgram core_map_MapNode_str_mir_MirProgram;
struct core_map_Map_str_mir_MirProgram { core_map_MapNode_str_mir_MirProgram** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_mir_MirProgram core_map_Map_str_mir_MirProgram;
__attribute__((hot)) mir_MirProgram** core_alloc_alloc_mir_MirProgram(long long count);
__attribute__((hot)) mir_MirProgram** core_alloc_resize_mir_MirProgram(mir_MirProgram** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_MirProgram(mir_MirProgram** ptr);
__attribute__((hot)) mir_MirProgram*** core_alloc_alloc_mir_MirProgram_ptr(long long count);
__attribute__((hot)) mir_MirProgram*** core_alloc_resize_mir_MirProgram_ptr(mir_MirProgram*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_MirProgram_ptr(mir_MirProgram*** ptr);
__attribute__((hot)) core_map_MapNode_str_mir_MirProgram** core_alloc_alloc_core_map_MapNode_str_mir_MirProgram(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_mir_MirProgram(core_map_MapNode_str_mir_MirProgram** ptr);

typedef MirBuilder mir_MirBuilder;
struct core_vec_Vec_mir_MirBuilder { mir_MirBuilder** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_MirBuilder core_vec_Vec_mir_MirBuilder;
struct core_vec_Vec_mir_MirBuilder_ptr { mir_MirBuilder*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_MirBuilder_ptr core_vec_Vec_mir_MirBuilder_ptr;
struct core_map_MapNode_str_mir_MirBuilder { char* key; mir_MirBuilder* value; struct core_map_MapNode_str_mir_MirBuilder* next; };
typedef struct core_map_MapNode_str_mir_MirBuilder core_map_MapNode_str_mir_MirBuilder;
struct core_map_Map_str_mir_MirBuilder { core_map_MapNode_str_mir_MirBuilder** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_mir_MirBuilder core_map_Map_str_mir_MirBuilder;
__attribute__((hot)) mir_MirBuilder** core_alloc_alloc_mir_MirBuilder(long long count);
__attribute__((hot)) mir_MirBuilder** core_alloc_resize_mir_MirBuilder(mir_MirBuilder** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_MirBuilder(mir_MirBuilder** ptr);
__attribute__((hot)) mir_MirBuilder*** core_alloc_alloc_mir_MirBuilder_ptr(long long count);
__attribute__((hot)) mir_MirBuilder*** core_alloc_resize_mir_MirBuilder_ptr(mir_MirBuilder*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_MirBuilder_ptr(mir_MirBuilder*** ptr);
__attribute__((hot)) core_map_MapNode_str_mir_MirBuilder** core_alloc_alloc_core_map_MapNode_str_mir_MirBuilder(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_mir_MirBuilder(core_map_MapNode_str_mir_MirBuilder** ptr);

typedef LiveSet mir_LiveSet;
struct core_vec_Vec_mir_LiveSet { mir_LiveSet** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_LiveSet core_vec_Vec_mir_LiveSet;
struct core_vec_Vec_mir_LiveSet_ptr { mir_LiveSet*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_mir_LiveSet_ptr core_vec_Vec_mir_LiveSet_ptr;
struct core_map_MapNode_str_mir_LiveSet { char* key; mir_LiveSet* value; struct core_map_MapNode_str_mir_LiveSet* next; };
typedef struct core_map_MapNode_str_mir_LiveSet core_map_MapNode_str_mir_LiveSet;
struct core_map_Map_str_mir_LiveSet { core_map_MapNode_str_mir_LiveSet** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_mir_LiveSet core_map_Map_str_mir_LiveSet;
__attribute__((hot)) mir_LiveSet** core_alloc_alloc_mir_LiveSet(long long count);
__attribute__((hot)) mir_LiveSet** core_alloc_resize_mir_LiveSet(mir_LiveSet** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_LiveSet(mir_LiveSet** ptr);
__attribute__((hot)) mir_LiveSet*** core_alloc_alloc_mir_LiveSet_ptr(long long count);
__attribute__((hot)) mir_LiveSet*** core_alloc_resize_mir_LiveSet_ptr(mir_LiveSet*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_mir_LiveSet_ptr(mir_LiveSet*** ptr);
__attribute__((hot)) core_map_MapNode_str_mir_LiveSet** core_alloc_alloc_core_map_MapNode_str_mir_LiveSet(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_mir_LiveSet(core_map_MapNode_str_mir_LiveSet** ptr);

typedef SymbolKind sema_SymbolKind;
struct core_vec_Vec_sema_SymbolKind { sema_SymbolKind* data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_SymbolKind core_vec_Vec_sema_SymbolKind;
struct core_vec_Vec_sema_SymbolKind_ptr { sema_SymbolKind** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_SymbolKind_ptr core_vec_Vec_sema_SymbolKind_ptr;
__attribute__((hot)) sema_SymbolKind* core_alloc_alloc_sema_SymbolKind(long long count);
__attribute__((hot)) sema_SymbolKind* core_alloc_resize_sema_SymbolKind(sema_SymbolKind* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_SymbolKind(sema_SymbolKind* ptr);
__attribute__((hot)) sema_SymbolKind** core_alloc_alloc_sema_SymbolKind_ptr(long long count);
__attribute__((hot)) sema_SymbolKind** core_alloc_resize_sema_SymbolKind_ptr(sema_SymbolKind** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_SymbolKind_ptr(sema_SymbolKind** ptr);

typedef Symbol sema_Symbol;
struct core_vec_Vec_sema_Symbol { sema_Symbol** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_Symbol core_vec_Vec_sema_Symbol;
struct core_vec_Vec_sema_Symbol_ptr { sema_Symbol*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_Symbol_ptr core_vec_Vec_sema_Symbol_ptr;
struct core_map_MapNode_str_sema_Symbol { char* key; sema_Symbol* value; struct core_map_MapNode_str_sema_Symbol* next; };
typedef struct core_map_MapNode_str_sema_Symbol core_map_MapNode_str_sema_Symbol;
struct core_map_Map_str_sema_Symbol { core_map_MapNode_str_sema_Symbol** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_sema_Symbol core_map_Map_str_sema_Symbol;
__attribute__((hot)) sema_Symbol** core_alloc_alloc_sema_Symbol(long long count);
__attribute__((hot)) sema_Symbol** core_alloc_resize_sema_Symbol(sema_Symbol** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_Symbol(sema_Symbol** ptr);
__attribute__((hot)) sema_Symbol*** core_alloc_alloc_sema_Symbol_ptr(long long count);
__attribute__((hot)) sema_Symbol*** core_alloc_resize_sema_Symbol_ptr(sema_Symbol*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_Symbol_ptr(sema_Symbol*** ptr);
__attribute__((hot)) core_map_MapNode_str_sema_Symbol** core_alloc_alloc_core_map_MapNode_str_sema_Symbol(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_sema_Symbol(core_map_MapNode_str_sema_Symbol** ptr);

typedef Scope sema_Scope;
struct core_vec_Vec_sema_Scope { sema_Scope** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_Scope core_vec_Vec_sema_Scope;
struct core_vec_Vec_sema_Scope_ptr { sema_Scope*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_Scope_ptr core_vec_Vec_sema_Scope_ptr;
struct core_map_MapNode_str_sema_Scope { char* key; sema_Scope* value; struct core_map_MapNode_str_sema_Scope* next; };
typedef struct core_map_MapNode_str_sema_Scope core_map_MapNode_str_sema_Scope;
struct core_map_Map_str_sema_Scope { core_map_MapNode_str_sema_Scope** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_sema_Scope core_map_Map_str_sema_Scope;
__attribute__((hot)) sema_Scope** core_alloc_alloc_sema_Scope(long long count);
__attribute__((hot)) sema_Scope** core_alloc_resize_sema_Scope(sema_Scope** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_Scope(sema_Scope** ptr);
__attribute__((hot)) sema_Scope*** core_alloc_alloc_sema_Scope_ptr(long long count);
__attribute__((hot)) sema_Scope*** core_alloc_resize_sema_Scope_ptr(sema_Scope*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_Scope_ptr(sema_Scope*** ptr);
__attribute__((hot)) core_map_MapNode_str_sema_Scope** core_alloc_alloc_core_map_MapNode_str_sema_Scope(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_sema_Scope(core_map_MapNode_str_sema_Scope** ptr);

typedef Sema sema_Sema;
struct core_vec_Vec_sema_Sema { sema_Sema** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_Sema core_vec_Vec_sema_Sema;
struct core_vec_Vec_sema_Sema_ptr { sema_Sema*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_sema_Sema_ptr core_vec_Vec_sema_Sema_ptr;
struct core_map_MapNode_str_sema_Sema { char* key; sema_Sema* value; struct core_map_MapNode_str_sema_Sema* next; };
typedef struct core_map_MapNode_str_sema_Sema core_map_MapNode_str_sema_Sema;
struct core_map_Map_str_sema_Sema { core_map_MapNode_str_sema_Sema** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_sema_Sema core_map_Map_str_sema_Sema;
__attribute__((hot)) sema_Sema** core_alloc_alloc_sema_Sema(long long count);
__attribute__((hot)) sema_Sema** core_alloc_resize_sema_Sema(sema_Sema** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_Sema(sema_Sema** ptr);
__attribute__((hot)) sema_Sema*** core_alloc_alloc_sema_Sema_ptr(long long count);
__attribute__((hot)) sema_Sema*** core_alloc_resize_sema_Sema_ptr(sema_Sema*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_sema_Sema_ptr(sema_Sema*** ptr);
__attribute__((hot)) core_map_MapNode_str_sema_Sema** core_alloc_alloc_core_map_MapNode_str_sema_Sema(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_sema_Sema(core_map_MapNode_str_sema_Sema** ptr);

typedef Formatter fmt_Formatter;
struct core_vec_Vec_fmt_Formatter { fmt_Formatter** data; long long len; long long capacity; };
typedef struct core_vec_Vec_fmt_Formatter core_vec_Vec_fmt_Formatter;
struct core_vec_Vec_fmt_Formatter_ptr { fmt_Formatter*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_fmt_Formatter_ptr core_vec_Vec_fmt_Formatter_ptr;
struct core_map_MapNode_str_fmt_Formatter { char* key; fmt_Formatter* value; struct core_map_MapNode_str_fmt_Formatter* next; };
typedef struct core_map_MapNode_str_fmt_Formatter core_map_MapNode_str_fmt_Formatter;
struct core_map_Map_str_fmt_Formatter { core_map_MapNode_str_fmt_Formatter** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_fmt_Formatter core_map_Map_str_fmt_Formatter;
__attribute__((hot)) fmt_Formatter** core_alloc_alloc_fmt_Formatter(long long count);
__attribute__((hot)) fmt_Formatter** core_alloc_resize_fmt_Formatter(fmt_Formatter** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_fmt_Formatter(fmt_Formatter** ptr);
__attribute__((hot)) fmt_Formatter*** core_alloc_alloc_fmt_Formatter_ptr(long long count);
__attribute__((hot)) fmt_Formatter*** core_alloc_resize_fmt_Formatter_ptr(fmt_Formatter*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_fmt_Formatter_ptr(fmt_Formatter*** ptr);
__attribute__((hot)) core_map_MapNode_str_fmt_Formatter** core_alloc_alloc_core_map_MapNode_str_fmt_Formatter(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_fmt_Formatter(core_map_MapNode_str_fmt_Formatter** ptr);

typedef CTok bindgen_CTok;
struct core_vec_Vec_bindgen_CTok { bindgen_CTok** data; long long len; long long capacity; };
typedef struct core_vec_Vec_bindgen_CTok core_vec_Vec_bindgen_CTok;
struct core_vec_Vec_bindgen_CTok_ptr { bindgen_CTok*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_bindgen_CTok_ptr core_vec_Vec_bindgen_CTok_ptr;
struct core_map_MapNode_str_bindgen_CTok { char* key; bindgen_CTok* value; struct core_map_MapNode_str_bindgen_CTok* next; };
typedef struct core_map_MapNode_str_bindgen_CTok core_map_MapNode_str_bindgen_CTok;
struct core_map_Map_str_bindgen_CTok { core_map_MapNode_str_bindgen_CTok** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_bindgen_CTok core_map_Map_str_bindgen_CTok;
__attribute__((hot)) bindgen_CTok** core_alloc_alloc_bindgen_CTok(long long count);
__attribute__((hot)) bindgen_CTok** core_alloc_resize_bindgen_CTok(bindgen_CTok** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_bindgen_CTok(bindgen_CTok** ptr);
__attribute__((hot)) bindgen_CTok*** core_alloc_alloc_bindgen_CTok_ptr(long long count);
__attribute__((hot)) bindgen_CTok*** core_alloc_resize_bindgen_CTok_ptr(bindgen_CTok*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_bindgen_CTok_ptr(bindgen_CTok*** ptr);
__attribute__((hot)) core_map_MapNode_str_bindgen_CTok** core_alloc_alloc_core_map_MapNode_str_bindgen_CTok(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_bindgen_CTok(core_map_MapNode_str_bindgen_CTok** ptr);

typedef Bindgen bindgen_Bindgen;
struct core_vec_Vec_bindgen_Bindgen { bindgen_Bindgen** data; long long len; long long capacity; };
typedef struct core_vec_Vec_bindgen_Bindgen core_vec_Vec_bindgen_Bindgen;
struct core_vec_Vec_bindgen_Bindgen_ptr { bindgen_Bindgen*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_bindgen_Bindgen_ptr core_vec_Vec_bindgen_Bindgen_ptr;
struct core_map_MapNode_str_bindgen_Bindgen { char* key; bindgen_Bindgen* value; struct core_map_MapNode_str_bindgen_Bindgen* next; };
typedef struct core_map_MapNode_str_bindgen_Bindgen core_map_MapNode_str_bindgen_Bindgen;
struct core_map_Map_str_bindgen_Bindgen { core_map_MapNode_str_bindgen_Bindgen** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_bindgen_Bindgen core_map_Map_str_bindgen_Bindgen;
__attribute__((hot)) bindgen_Bindgen** core_alloc_alloc_bindgen_Bindgen(long long count);
__attribute__((hot)) bindgen_Bindgen** core_alloc_resize_bindgen_Bindgen(bindgen_Bindgen** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_bindgen_Bindgen(bindgen_Bindgen** ptr);
__attribute__((hot)) bindgen_Bindgen*** core_alloc_alloc_bindgen_Bindgen_ptr(long long count);
__attribute__((hot)) bindgen_Bindgen*** core_alloc_resize_bindgen_Bindgen_ptr(bindgen_Bindgen*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_bindgen_Bindgen_ptr(bindgen_Bindgen*** ptr);
__attribute__((hot)) core_map_MapNode_str_bindgen_Bindgen** core_alloc_alloc_core_map_MapNode_str_bindgen_Bindgen(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_bindgen_Bindgen(core_map_MapNode_str_bindgen_Bindgen** ptr);

typedef CppType bindgen_CppType;
struct core_vec_Vec_bindgen_CppType { bindgen_CppType** data; long long len; long long capacity; };
typedef struct core_vec_Vec_bindgen_CppType core_vec_Vec_bindgen_CppType;
struct core_vec_Vec_bindgen_CppType_ptr { bindgen_CppType*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_bindgen_CppType_ptr core_vec_Vec_bindgen_CppType_ptr;
struct core_map_MapNode_str_bindgen_CppType { char* key; bindgen_CppType* value; struct core_map_MapNode_str_bindgen_CppType* next; };
typedef struct core_map_MapNode_str_bindgen_CppType core_map_MapNode_str_bindgen_CppType;
struct core_map_Map_str_bindgen_CppType { core_map_MapNode_str_bindgen_CppType** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_bindgen_CppType core_map_Map_str_bindgen_CppType;
__attribute__((hot)) bindgen_CppType** core_alloc_alloc_bindgen_CppType(long long count);
__attribute__((hot)) bindgen_CppType** core_alloc_resize_bindgen_CppType(bindgen_CppType** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_bindgen_CppType(bindgen_CppType** ptr);
__attribute__((hot)) bindgen_CppType*** core_alloc_alloc_bindgen_CppType_ptr(long long count);
__attribute__((hot)) bindgen_CppType*** core_alloc_resize_bindgen_CppType_ptr(bindgen_CppType*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_bindgen_CppType_ptr(bindgen_CppType*** ptr);
__attribute__((hot)) core_map_MapNode_str_bindgen_CppType** core_alloc_alloc_core_map_MapNode_str_bindgen_CppType(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_bindgen_CppType(core_map_MapNode_str_bindgen_CppType** ptr);

typedef CGenerator codegen_c_CGenerator;
struct core_vec_Vec_codegen_c_CGenerator { codegen_c_CGenerator** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_c_CGenerator core_vec_Vec_codegen_c_CGenerator;
struct core_vec_Vec_codegen_c_CGenerator_ptr { codegen_c_CGenerator*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_c_CGenerator_ptr core_vec_Vec_codegen_c_CGenerator_ptr;
struct core_map_MapNode_str_codegen_c_CGenerator { char* key; codegen_c_CGenerator* value; struct core_map_MapNode_str_codegen_c_CGenerator* next; };
typedef struct core_map_MapNode_str_codegen_c_CGenerator core_map_MapNode_str_codegen_c_CGenerator;
struct core_map_Map_str_codegen_c_CGenerator { core_map_MapNode_str_codegen_c_CGenerator** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_c_CGenerator core_map_Map_str_codegen_c_CGenerator;
__attribute__((hot)) codegen_c_CGenerator** core_alloc_alloc_codegen_c_CGenerator(long long count);
__attribute__((hot)) codegen_c_CGenerator** core_alloc_resize_codegen_c_CGenerator(codegen_c_CGenerator** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_c_CGenerator(codegen_c_CGenerator** ptr);
__attribute__((hot)) codegen_c_CGenerator*** core_alloc_alloc_codegen_c_CGenerator_ptr(long long count);
__attribute__((hot)) codegen_c_CGenerator*** core_alloc_resize_codegen_c_CGenerator_ptr(codegen_c_CGenerator*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_c_CGenerator_ptr(codegen_c_CGenerator*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_c_CGenerator** core_alloc_alloc_core_map_MapNode_str_codegen_c_CGenerator(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_c_CGenerator(core_map_MapNode_str_codegen_c_CGenerator** ptr);

typedef CppExporter codegen_cpp_export_CppExporter;
struct core_vec_Vec_codegen_cpp_export_CppExporter { codegen_cpp_export_CppExporter** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_cpp_export_CppExporter core_vec_Vec_codegen_cpp_export_CppExporter;
struct core_vec_Vec_codegen_cpp_export_CppExporter_ptr { codegen_cpp_export_CppExporter*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_cpp_export_CppExporter_ptr core_vec_Vec_codegen_cpp_export_CppExporter_ptr;
struct core_map_MapNode_str_codegen_cpp_export_CppExporter { char* key; codegen_cpp_export_CppExporter* value; struct core_map_MapNode_str_codegen_cpp_export_CppExporter* next; };
typedef struct core_map_MapNode_str_codegen_cpp_export_CppExporter core_map_MapNode_str_codegen_cpp_export_CppExporter;
struct core_map_Map_str_codegen_cpp_export_CppExporter { core_map_MapNode_str_codegen_cpp_export_CppExporter** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_cpp_export_CppExporter core_map_Map_str_codegen_cpp_export_CppExporter;
__attribute__((hot)) codegen_cpp_export_CppExporter** core_alloc_alloc_codegen_cpp_export_CppExporter(long long count);
__attribute__((hot)) codegen_cpp_export_CppExporter** core_alloc_resize_codegen_cpp_export_CppExporter(codegen_cpp_export_CppExporter** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_cpp_export_CppExporter(codegen_cpp_export_CppExporter** ptr);
__attribute__((hot)) codegen_cpp_export_CppExporter*** core_alloc_alloc_codegen_cpp_export_CppExporter_ptr(long long count);
__attribute__((hot)) codegen_cpp_export_CppExporter*** core_alloc_resize_codegen_cpp_export_CppExporter_ptr(codegen_cpp_export_CppExporter*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_cpp_export_CppExporter_ptr(codegen_cpp_export_CppExporter*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_cpp_export_CppExporter** core_alloc_alloc_core_map_MapNode_str_codegen_cpp_export_CppExporter(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_cpp_export_CppExporter(core_map_MapNode_str_codegen_cpp_export_CppExporter** ptr);

typedef LType taumir_ir_LType;
struct core_vec_Vec_taumir_ir_LType { taumir_ir_LType* data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_LType core_vec_Vec_taumir_ir_LType;
struct core_vec_Vec_taumir_ir_LType_ptr { taumir_ir_LType** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_LType_ptr core_vec_Vec_taumir_ir_LType_ptr;
__attribute__((hot)) taumir_ir_LType* core_alloc_alloc_taumir_ir_LType(long long count);
__attribute__((hot)) taumir_ir_LType* core_alloc_resize_taumir_ir_LType(taumir_ir_LType* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_LType(taumir_ir_LType* ptr);
__attribute__((hot)) taumir_ir_LType** core_alloc_alloc_taumir_ir_LType_ptr(long long count);
__attribute__((hot)) taumir_ir_LType** core_alloc_resize_taumir_ir_LType_ptr(taumir_ir_LType** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_LType_ptr(taumir_ir_LType** ptr);

typedef LInst taumir_ir_LInst;
struct core_vec_Vec_taumir_ir_LInst { taumir_ir_LInst* data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_LInst core_vec_Vec_taumir_ir_LInst;
struct core_vec_Vec_taumir_ir_LInst_ptr { taumir_ir_LInst** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_LInst_ptr core_vec_Vec_taumir_ir_LInst_ptr;
__attribute__((hot)) taumir_ir_LInst* core_alloc_alloc_taumir_ir_LInst(long long count);
__attribute__((hot)) taumir_ir_LInst* core_alloc_resize_taumir_ir_LInst(taumir_ir_LInst* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_LInst(taumir_ir_LInst* ptr);
__attribute__((hot)) taumir_ir_LInst** core_alloc_alloc_taumir_ir_LInst_ptr(long long count);
__attribute__((hot)) taumir_ir_LInst** core_alloc_resize_taumir_ir_LInst_ptr(taumir_ir_LInst** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_LInst_ptr(taumir_ir_LInst** ptr);

typedef LTerm taumir_ir_LTerm;
struct core_vec_Vec_taumir_ir_LTerm { taumir_ir_LTerm* data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_LTerm core_vec_Vec_taumir_ir_LTerm;
struct core_vec_Vec_taumir_ir_LTerm_ptr { taumir_ir_LTerm** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_LTerm_ptr core_vec_Vec_taumir_ir_LTerm_ptr;
__attribute__((hot)) taumir_ir_LTerm* core_alloc_alloc_taumir_ir_LTerm(long long count);
__attribute__((hot)) taumir_ir_LTerm* core_alloc_resize_taumir_ir_LTerm(taumir_ir_LTerm* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_LTerm(taumir_ir_LTerm* ptr);
__attribute__((hot)) taumir_ir_LTerm** core_alloc_alloc_taumir_ir_LTerm_ptr(long long count);
__attribute__((hot)) taumir_ir_LTerm** core_alloc_resize_taumir_ir_LTerm_ptr(taumir_ir_LTerm** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_LTerm_ptr(taumir_ir_LTerm** ptr);

typedef LBlock taumir_ir_LBlock;
struct core_vec_Vec_taumir_ir_LBlock { taumir_ir_LBlock** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_LBlock core_vec_Vec_taumir_ir_LBlock;
struct core_vec_Vec_taumir_ir_LBlock_ptr { taumir_ir_LBlock*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_LBlock_ptr core_vec_Vec_taumir_ir_LBlock_ptr;
struct core_map_MapNode_str_taumir_ir_LBlock { char* key; taumir_ir_LBlock* value; struct core_map_MapNode_str_taumir_ir_LBlock* next; };
typedef struct core_map_MapNode_str_taumir_ir_LBlock core_map_MapNode_str_taumir_ir_LBlock;
struct core_map_Map_str_taumir_ir_LBlock { core_map_MapNode_str_taumir_ir_LBlock** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_taumir_ir_LBlock core_map_Map_str_taumir_ir_LBlock;
__attribute__((hot)) taumir_ir_LBlock** core_alloc_alloc_taumir_ir_LBlock(long long count);
__attribute__((hot)) taumir_ir_LBlock** core_alloc_resize_taumir_ir_LBlock(taumir_ir_LBlock** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_LBlock(taumir_ir_LBlock** ptr);
__attribute__((hot)) taumir_ir_LBlock*** core_alloc_alloc_taumir_ir_LBlock_ptr(long long count);
__attribute__((hot)) taumir_ir_LBlock*** core_alloc_resize_taumir_ir_LBlock_ptr(taumir_ir_LBlock*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_LBlock_ptr(taumir_ir_LBlock*** ptr);
__attribute__((hot)) core_map_MapNode_str_taumir_ir_LBlock** core_alloc_alloc_core_map_MapNode_str_taumir_ir_LBlock(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_taumir_ir_LBlock(core_map_MapNode_str_taumir_ir_LBlock** ptr);

typedef LFunc taumir_ir_LFunc;
struct core_vec_Vec_taumir_ir_LFunc { taumir_ir_LFunc** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_LFunc core_vec_Vec_taumir_ir_LFunc;
struct core_vec_Vec_taumir_ir_LFunc_ptr { taumir_ir_LFunc*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_LFunc_ptr core_vec_Vec_taumir_ir_LFunc_ptr;
struct core_map_MapNode_str_taumir_ir_LFunc { char* key; taumir_ir_LFunc* value; struct core_map_MapNode_str_taumir_ir_LFunc* next; };
typedef struct core_map_MapNode_str_taumir_ir_LFunc core_map_MapNode_str_taumir_ir_LFunc;
struct core_map_Map_str_taumir_ir_LFunc { core_map_MapNode_str_taumir_ir_LFunc** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_taumir_ir_LFunc core_map_Map_str_taumir_ir_LFunc;
__attribute__((hot)) taumir_ir_LFunc** core_alloc_alloc_taumir_ir_LFunc(long long count);
__attribute__((hot)) taumir_ir_LFunc** core_alloc_resize_taumir_ir_LFunc(taumir_ir_LFunc** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_LFunc(taumir_ir_LFunc** ptr);
__attribute__((hot)) taumir_ir_LFunc*** core_alloc_alloc_taumir_ir_LFunc_ptr(long long count);
__attribute__((hot)) taumir_ir_LFunc*** core_alloc_resize_taumir_ir_LFunc_ptr(taumir_ir_LFunc*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_LFunc_ptr(taumir_ir_LFunc*** ptr);
__attribute__((hot)) core_map_MapNode_str_taumir_ir_LFunc** core_alloc_alloc_core_map_MapNode_str_taumir_ir_LFunc(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_taumir_ir_LFunc(core_map_MapNode_str_taumir_ir_LFunc** ptr);

typedef ClassLayout taumir_ir_ClassLayout;
struct core_vec_Vec_taumir_ir_ClassLayout { taumir_ir_ClassLayout** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_ClassLayout core_vec_Vec_taumir_ir_ClassLayout;
struct core_vec_Vec_taumir_ir_ClassLayout_ptr { taumir_ir_ClassLayout*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_ClassLayout_ptr core_vec_Vec_taumir_ir_ClassLayout_ptr;
struct core_map_MapNode_str_taumir_ir_ClassLayout { char* key; taumir_ir_ClassLayout* value; struct core_map_MapNode_str_taumir_ir_ClassLayout* next; };
typedef struct core_map_MapNode_str_taumir_ir_ClassLayout core_map_MapNode_str_taumir_ir_ClassLayout;
struct core_map_Map_str_taumir_ir_ClassLayout { core_map_MapNode_str_taumir_ir_ClassLayout** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_taumir_ir_ClassLayout core_map_Map_str_taumir_ir_ClassLayout;
__attribute__((hot)) taumir_ir_ClassLayout** core_alloc_alloc_taumir_ir_ClassLayout(long long count);
__attribute__((hot)) taumir_ir_ClassLayout** core_alloc_resize_taumir_ir_ClassLayout(taumir_ir_ClassLayout** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_ClassLayout(taumir_ir_ClassLayout** ptr);
__attribute__((hot)) taumir_ir_ClassLayout*** core_alloc_alloc_taumir_ir_ClassLayout_ptr(long long count);
__attribute__((hot)) taumir_ir_ClassLayout*** core_alloc_resize_taumir_ir_ClassLayout_ptr(taumir_ir_ClassLayout*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_ClassLayout_ptr(taumir_ir_ClassLayout*** ptr);
__attribute__((hot)) core_map_MapNode_str_taumir_ir_ClassLayout** core_alloc_alloc_core_map_MapNode_str_taumir_ir_ClassLayout(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_taumir_ir_ClassLayout(core_map_MapNode_str_taumir_ir_ClassLayout** ptr);

typedef VariantLayout taumir_ir_VariantLayout;
struct core_vec_Vec_taumir_ir_VariantLayout { taumir_ir_VariantLayout** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_VariantLayout core_vec_Vec_taumir_ir_VariantLayout;
struct core_vec_Vec_taumir_ir_VariantLayout_ptr { taumir_ir_VariantLayout*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_VariantLayout_ptr core_vec_Vec_taumir_ir_VariantLayout_ptr;
struct core_map_MapNode_str_taumir_ir_VariantLayout { char* key; taumir_ir_VariantLayout* value; struct core_map_MapNode_str_taumir_ir_VariantLayout* next; };
typedef struct core_map_MapNode_str_taumir_ir_VariantLayout core_map_MapNode_str_taumir_ir_VariantLayout;
struct core_map_Map_str_taumir_ir_VariantLayout { core_map_MapNode_str_taumir_ir_VariantLayout** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_taumir_ir_VariantLayout core_map_Map_str_taumir_ir_VariantLayout;
__attribute__((hot)) taumir_ir_VariantLayout** core_alloc_alloc_taumir_ir_VariantLayout(long long count);
__attribute__((hot)) taumir_ir_VariantLayout** core_alloc_resize_taumir_ir_VariantLayout(taumir_ir_VariantLayout** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_VariantLayout(taumir_ir_VariantLayout** ptr);
__attribute__((hot)) taumir_ir_VariantLayout*** core_alloc_alloc_taumir_ir_VariantLayout_ptr(long long count);
__attribute__((hot)) taumir_ir_VariantLayout*** core_alloc_resize_taumir_ir_VariantLayout_ptr(taumir_ir_VariantLayout*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_VariantLayout_ptr(taumir_ir_VariantLayout*** ptr);
__attribute__((hot)) core_map_MapNode_str_taumir_ir_VariantLayout** core_alloc_alloc_core_map_MapNode_str_taumir_ir_VariantLayout(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_taumir_ir_VariantLayout(core_map_MapNode_str_taumir_ir_VariantLayout** ptr);

typedef EnumLayout taumir_ir_EnumLayout;
struct core_vec_Vec_taumir_ir_EnumLayout { taumir_ir_EnumLayout** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_EnumLayout core_vec_Vec_taumir_ir_EnumLayout;
struct core_vec_Vec_taumir_ir_EnumLayout_ptr { taumir_ir_EnumLayout*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_EnumLayout_ptr core_vec_Vec_taumir_ir_EnumLayout_ptr;
struct core_map_MapNode_str_taumir_ir_EnumLayout { char* key; taumir_ir_EnumLayout* value; struct core_map_MapNode_str_taumir_ir_EnumLayout* next; };
typedef struct core_map_MapNode_str_taumir_ir_EnumLayout core_map_MapNode_str_taumir_ir_EnumLayout;
struct core_map_Map_str_taumir_ir_EnumLayout { core_map_MapNode_str_taumir_ir_EnumLayout** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_taumir_ir_EnumLayout core_map_Map_str_taumir_ir_EnumLayout;
__attribute__((hot)) taumir_ir_EnumLayout** core_alloc_alloc_taumir_ir_EnumLayout(long long count);
__attribute__((hot)) taumir_ir_EnumLayout** core_alloc_resize_taumir_ir_EnumLayout(taumir_ir_EnumLayout** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_EnumLayout(taumir_ir_EnumLayout** ptr);
__attribute__((hot)) taumir_ir_EnumLayout*** core_alloc_alloc_taumir_ir_EnumLayout_ptr(long long count);
__attribute__((hot)) taumir_ir_EnumLayout*** core_alloc_resize_taumir_ir_EnumLayout_ptr(taumir_ir_EnumLayout*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_EnumLayout_ptr(taumir_ir_EnumLayout*** ptr);
__attribute__((hot)) core_map_MapNode_str_taumir_ir_EnumLayout** core_alloc_alloc_core_map_MapNode_str_taumir_ir_EnumLayout(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_taumir_ir_EnumLayout(core_map_MapNode_str_taumir_ir_EnumLayout** ptr);

typedef LModule taumir_ir_LModule;
struct core_vec_Vec_taumir_ir_LModule { taumir_ir_LModule** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_LModule core_vec_Vec_taumir_ir_LModule;
struct core_vec_Vec_taumir_ir_LModule_ptr { taumir_ir_LModule*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_taumir_ir_LModule_ptr core_vec_Vec_taumir_ir_LModule_ptr;
struct core_map_MapNode_str_taumir_ir_LModule { char* key; taumir_ir_LModule* value; struct core_map_MapNode_str_taumir_ir_LModule* next; };
typedef struct core_map_MapNode_str_taumir_ir_LModule core_map_MapNode_str_taumir_ir_LModule;
struct core_map_Map_str_taumir_ir_LModule { core_map_MapNode_str_taumir_ir_LModule** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_taumir_ir_LModule core_map_Map_str_taumir_ir_LModule;
__attribute__((hot)) taumir_ir_LModule** core_alloc_alloc_taumir_ir_LModule(long long count);
__attribute__((hot)) taumir_ir_LModule** core_alloc_resize_taumir_ir_LModule(taumir_ir_LModule** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_LModule(taumir_ir_LModule** ptr);
__attribute__((hot)) taumir_ir_LModule*** core_alloc_alloc_taumir_ir_LModule_ptr(long long count);
__attribute__((hot)) taumir_ir_LModule*** core_alloc_resize_taumir_ir_LModule_ptr(taumir_ir_LModule*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_taumir_ir_LModule_ptr(taumir_ir_LModule*** ptr);
__attribute__((hot)) core_map_MapNode_str_taumir_ir_LModule** core_alloc_alloc_core_map_MapNode_str_taumir_ir_LModule(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_taumir_ir_LModule(core_map_MapNode_str_taumir_ir_LModule** ptr);

typedef LlvmEmitter codegen_llvm_emit_LlvmEmitter;
struct core_vec_Vec_codegen_llvm_emit_LlvmEmitter { codegen_llvm_emit_LlvmEmitter** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_llvm_emit_LlvmEmitter core_vec_Vec_codegen_llvm_emit_LlvmEmitter;
struct core_vec_Vec_codegen_llvm_emit_LlvmEmitter_ptr { codegen_llvm_emit_LlvmEmitter*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_llvm_emit_LlvmEmitter_ptr core_vec_Vec_codegen_llvm_emit_LlvmEmitter_ptr;
struct core_map_MapNode_str_codegen_llvm_emit_LlvmEmitter { char* key; codegen_llvm_emit_LlvmEmitter* value; struct core_map_MapNode_str_codegen_llvm_emit_LlvmEmitter* next; };
typedef struct core_map_MapNode_str_codegen_llvm_emit_LlvmEmitter core_map_MapNode_str_codegen_llvm_emit_LlvmEmitter;
struct core_map_Map_str_codegen_llvm_emit_LlvmEmitter { core_map_MapNode_str_codegen_llvm_emit_LlvmEmitter** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_llvm_emit_LlvmEmitter core_map_Map_str_codegen_llvm_emit_LlvmEmitter;
__attribute__((hot)) codegen_llvm_emit_LlvmEmitter** core_alloc_alloc_codegen_llvm_emit_LlvmEmitter(long long count);
__attribute__((hot)) codegen_llvm_emit_LlvmEmitter** core_alloc_resize_codegen_llvm_emit_LlvmEmitter(codegen_llvm_emit_LlvmEmitter** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_llvm_emit_LlvmEmitter(codegen_llvm_emit_LlvmEmitter** ptr);
__attribute__((hot)) codegen_llvm_emit_LlvmEmitter*** core_alloc_alloc_codegen_llvm_emit_LlvmEmitter_ptr(long long count);
__attribute__((hot)) codegen_llvm_emit_LlvmEmitter*** core_alloc_resize_codegen_llvm_emit_LlvmEmitter_ptr(codegen_llvm_emit_LlvmEmitter*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_llvm_emit_LlvmEmitter_ptr(codegen_llvm_emit_LlvmEmitter*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_llvm_emit_LlvmEmitter** core_alloc_alloc_core_map_MapNode_str_codegen_llvm_emit_LlvmEmitter(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_llvm_emit_LlvmEmitter(core_map_MapNode_str_codegen_llvm_emit_LlvmEmitter** ptr);

typedef LlvmGenerator codegen_llvm_LlvmGenerator;
struct core_vec_Vec_codegen_llvm_LlvmGenerator { codegen_llvm_LlvmGenerator** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_llvm_LlvmGenerator core_vec_Vec_codegen_llvm_LlvmGenerator;
struct core_vec_Vec_codegen_llvm_LlvmGenerator_ptr { codegen_llvm_LlvmGenerator*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_llvm_LlvmGenerator_ptr core_vec_Vec_codegen_llvm_LlvmGenerator_ptr;
struct core_map_MapNode_str_codegen_llvm_LlvmGenerator { char* key; codegen_llvm_LlvmGenerator* value; struct core_map_MapNode_str_codegen_llvm_LlvmGenerator* next; };
typedef struct core_map_MapNode_str_codegen_llvm_LlvmGenerator core_map_MapNode_str_codegen_llvm_LlvmGenerator;
struct core_map_Map_str_codegen_llvm_LlvmGenerator { core_map_MapNode_str_codegen_llvm_LlvmGenerator** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_llvm_LlvmGenerator core_map_Map_str_codegen_llvm_LlvmGenerator;
__attribute__((hot)) codegen_llvm_LlvmGenerator** core_alloc_alloc_codegen_llvm_LlvmGenerator(long long count);
__attribute__((hot)) codegen_llvm_LlvmGenerator** core_alloc_resize_codegen_llvm_LlvmGenerator(codegen_llvm_LlvmGenerator** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_llvm_LlvmGenerator(codegen_llvm_LlvmGenerator** ptr);
__attribute__((hot)) codegen_llvm_LlvmGenerator*** core_alloc_alloc_codegen_llvm_LlvmGenerator_ptr(long long count);
__attribute__((hot)) codegen_llvm_LlvmGenerator*** core_alloc_resize_codegen_llvm_LlvmGenerator_ptr(codegen_llvm_LlvmGenerator*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_llvm_LlvmGenerator_ptr(codegen_llvm_LlvmGenerator*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_llvm_LlvmGenerator** core_alloc_alloc_core_map_MapNode_str_codegen_llvm_LlvmGenerator(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_llvm_LlvmGenerator(core_map_MapNode_str_codegen_llvm_LlvmGenerator** ptr);

typedef ByteBuf codegen_native_bytebuf_ByteBuf;
struct core_vec_Vec_codegen_native_bytebuf_ByteBuf { codegen_native_bytebuf_ByteBuf** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_native_bytebuf_ByteBuf core_vec_Vec_codegen_native_bytebuf_ByteBuf;
struct core_vec_Vec_codegen_native_bytebuf_ByteBuf_ptr { codegen_native_bytebuf_ByteBuf*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_native_bytebuf_ByteBuf_ptr core_vec_Vec_codegen_native_bytebuf_ByteBuf_ptr;
struct core_map_MapNode_str_codegen_native_bytebuf_ByteBuf { char* key; codegen_native_bytebuf_ByteBuf* value; struct core_map_MapNode_str_codegen_native_bytebuf_ByteBuf* next; };
typedef struct core_map_MapNode_str_codegen_native_bytebuf_ByteBuf core_map_MapNode_str_codegen_native_bytebuf_ByteBuf;
struct core_map_Map_str_codegen_native_bytebuf_ByteBuf { core_map_MapNode_str_codegen_native_bytebuf_ByteBuf** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_native_bytebuf_ByteBuf core_map_Map_str_codegen_native_bytebuf_ByteBuf;
__attribute__((hot)) codegen_native_bytebuf_ByteBuf** core_alloc_alloc_codegen_native_bytebuf_ByteBuf(long long count);
__attribute__((hot)) codegen_native_bytebuf_ByteBuf** core_alloc_resize_codegen_native_bytebuf_ByteBuf(codegen_native_bytebuf_ByteBuf** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_native_bytebuf_ByteBuf(codegen_native_bytebuf_ByteBuf** ptr);
__attribute__((hot)) codegen_native_bytebuf_ByteBuf*** core_alloc_alloc_codegen_native_bytebuf_ByteBuf_ptr(long long count);
__attribute__((hot)) codegen_native_bytebuf_ByteBuf*** core_alloc_resize_codegen_native_bytebuf_ByteBuf_ptr(codegen_native_bytebuf_ByteBuf*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_native_bytebuf_ByteBuf_ptr(codegen_native_bytebuf_ByteBuf*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_native_bytebuf_ByteBuf** core_alloc_alloc_core_map_MapNode_str_codegen_native_bytebuf_ByteBuf(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_native_bytebuf_ByteBuf(core_map_MapNode_str_codegen_native_bytebuf_ByteBuf** ptr);

typedef Reloc codegen_native_isa_x64_Reloc;
struct core_vec_Vec_codegen_native_isa_x64_Reloc { codegen_native_isa_x64_Reloc** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_native_isa_x64_Reloc core_vec_Vec_codegen_native_isa_x64_Reloc;
struct core_vec_Vec_codegen_native_isa_x64_Reloc_ptr { codegen_native_isa_x64_Reloc*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_native_isa_x64_Reloc_ptr core_vec_Vec_codegen_native_isa_x64_Reloc_ptr;
struct core_map_MapNode_str_codegen_native_isa_x64_Reloc { char* key; codegen_native_isa_x64_Reloc* value; struct core_map_MapNode_str_codegen_native_isa_x64_Reloc* next; };
typedef struct core_map_MapNode_str_codegen_native_isa_x64_Reloc core_map_MapNode_str_codegen_native_isa_x64_Reloc;
struct core_map_Map_str_codegen_native_isa_x64_Reloc { core_map_MapNode_str_codegen_native_isa_x64_Reloc** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_native_isa_x64_Reloc core_map_Map_str_codegen_native_isa_x64_Reloc;
__attribute__((hot)) codegen_native_isa_x64_Reloc** core_alloc_alloc_codegen_native_isa_x64_Reloc(long long count);
__attribute__((hot)) codegen_native_isa_x64_Reloc** core_alloc_resize_codegen_native_isa_x64_Reloc(codegen_native_isa_x64_Reloc** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_native_isa_x64_Reloc(codegen_native_isa_x64_Reloc** ptr);
__attribute__((hot)) codegen_native_isa_x64_Reloc*** core_alloc_alloc_codegen_native_isa_x64_Reloc_ptr(long long count);
__attribute__((hot)) codegen_native_isa_x64_Reloc*** core_alloc_resize_codegen_native_isa_x64_Reloc_ptr(codegen_native_isa_x64_Reloc*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_native_isa_x64_Reloc_ptr(codegen_native_isa_x64_Reloc*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_native_isa_x64_Reloc** core_alloc_alloc_core_map_MapNode_str_codegen_native_isa_x64_Reloc(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_native_isa_x64_Reloc(core_map_MapNode_str_codegen_native_isa_x64_Reloc** ptr);

typedef Jump codegen_native_isa_x64_Jump;
struct core_vec_Vec_codegen_native_isa_x64_Jump { codegen_native_isa_x64_Jump** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_native_isa_x64_Jump core_vec_Vec_codegen_native_isa_x64_Jump;
struct core_vec_Vec_codegen_native_isa_x64_Jump_ptr { codegen_native_isa_x64_Jump*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_native_isa_x64_Jump_ptr core_vec_Vec_codegen_native_isa_x64_Jump_ptr;
struct core_map_MapNode_str_codegen_native_isa_x64_Jump { char* key; codegen_native_isa_x64_Jump* value; struct core_map_MapNode_str_codegen_native_isa_x64_Jump* next; };
typedef struct core_map_MapNode_str_codegen_native_isa_x64_Jump core_map_MapNode_str_codegen_native_isa_x64_Jump;
struct core_map_Map_str_codegen_native_isa_x64_Jump { core_map_MapNode_str_codegen_native_isa_x64_Jump** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_native_isa_x64_Jump core_map_Map_str_codegen_native_isa_x64_Jump;
__attribute__((hot)) codegen_native_isa_x64_Jump** core_alloc_alloc_codegen_native_isa_x64_Jump(long long count);
__attribute__((hot)) codegen_native_isa_x64_Jump** core_alloc_resize_codegen_native_isa_x64_Jump(codegen_native_isa_x64_Jump** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_native_isa_x64_Jump(codegen_native_isa_x64_Jump** ptr);
__attribute__((hot)) codegen_native_isa_x64_Jump*** core_alloc_alloc_codegen_native_isa_x64_Jump_ptr(long long count);
__attribute__((hot)) codegen_native_isa_x64_Jump*** core_alloc_resize_codegen_native_isa_x64_Jump_ptr(codegen_native_isa_x64_Jump*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_native_isa_x64_Jump_ptr(codegen_native_isa_x64_Jump*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_native_isa_x64_Jump** core_alloc_alloc_core_map_MapNode_str_codegen_native_isa_x64_Jump(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_native_isa_x64_Jump(core_map_MapNode_str_codegen_native_isa_x64_Jump** ptr);

typedef EncodedFunc codegen_native_isa_x64_EncodedFunc;
struct core_vec_Vec_codegen_native_isa_x64_EncodedFunc { codegen_native_isa_x64_EncodedFunc** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_native_isa_x64_EncodedFunc core_vec_Vec_codegen_native_isa_x64_EncodedFunc;
struct core_vec_Vec_codegen_native_isa_x64_EncodedFunc_ptr { codegen_native_isa_x64_EncodedFunc*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_native_isa_x64_EncodedFunc_ptr core_vec_Vec_codegen_native_isa_x64_EncodedFunc_ptr;
struct core_map_MapNode_str_codegen_native_isa_x64_EncodedFunc { char* key; codegen_native_isa_x64_EncodedFunc* value; struct core_map_MapNode_str_codegen_native_isa_x64_EncodedFunc* next; };
typedef struct core_map_MapNode_str_codegen_native_isa_x64_EncodedFunc core_map_MapNode_str_codegen_native_isa_x64_EncodedFunc;
struct core_map_Map_str_codegen_native_isa_x64_EncodedFunc { core_map_MapNode_str_codegen_native_isa_x64_EncodedFunc** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_native_isa_x64_EncodedFunc core_map_Map_str_codegen_native_isa_x64_EncodedFunc;
__attribute__((hot)) codegen_native_isa_x64_EncodedFunc** core_alloc_alloc_codegen_native_isa_x64_EncodedFunc(long long count);
__attribute__((hot)) codegen_native_isa_x64_EncodedFunc** core_alloc_resize_codegen_native_isa_x64_EncodedFunc(codegen_native_isa_x64_EncodedFunc** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_native_isa_x64_EncodedFunc(codegen_native_isa_x64_EncodedFunc** ptr);
__attribute__((hot)) codegen_native_isa_x64_EncodedFunc*** core_alloc_alloc_codegen_native_isa_x64_EncodedFunc_ptr(long long count);
__attribute__((hot)) codegen_native_isa_x64_EncodedFunc*** core_alloc_resize_codegen_native_isa_x64_EncodedFunc_ptr(codegen_native_isa_x64_EncodedFunc*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_native_isa_x64_EncodedFunc_ptr(codegen_native_isa_x64_EncodedFunc*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_native_isa_x64_EncodedFunc** core_alloc_alloc_core_map_MapNode_str_codegen_native_isa_x64_EncodedFunc(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_native_isa_x64_EncodedFunc(core_map_MapNode_str_codegen_native_isa_x64_EncodedFunc** ptr);

typedef TextReloc codegen_native_elf_TextReloc;
struct core_vec_Vec_codegen_native_elf_TextReloc { codegen_native_elf_TextReloc** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_native_elf_TextReloc core_vec_Vec_codegen_native_elf_TextReloc;
struct core_vec_Vec_codegen_native_elf_TextReloc_ptr { codegen_native_elf_TextReloc*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_native_elf_TextReloc_ptr core_vec_Vec_codegen_native_elf_TextReloc_ptr;
struct core_map_MapNode_str_codegen_native_elf_TextReloc { char* key; codegen_native_elf_TextReloc* value; struct core_map_MapNode_str_codegen_native_elf_TextReloc* next; };
typedef struct core_map_MapNode_str_codegen_native_elf_TextReloc core_map_MapNode_str_codegen_native_elf_TextReloc;
struct core_map_Map_str_codegen_native_elf_TextReloc { core_map_MapNode_str_codegen_native_elf_TextReloc** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_native_elf_TextReloc core_map_Map_str_codegen_native_elf_TextReloc;
__attribute__((hot)) codegen_native_elf_TextReloc** core_alloc_alloc_codegen_native_elf_TextReloc(long long count);
__attribute__((hot)) codegen_native_elf_TextReloc** core_alloc_resize_codegen_native_elf_TextReloc(codegen_native_elf_TextReloc** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_native_elf_TextReloc(codegen_native_elf_TextReloc** ptr);
__attribute__((hot)) codegen_native_elf_TextReloc*** core_alloc_alloc_codegen_native_elf_TextReloc_ptr(long long count);
__attribute__((hot)) codegen_native_elf_TextReloc*** core_alloc_resize_codegen_native_elf_TextReloc_ptr(codegen_native_elf_TextReloc*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_native_elf_TextReloc_ptr(codegen_native_elf_TextReloc*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_native_elf_TextReloc** core_alloc_alloc_core_map_MapNode_str_codegen_native_elf_TextReloc(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_native_elf_TextReloc(core_map_MapNode_str_codegen_native_elf_TextReloc** ptr);

typedef NativeGenerator codegen_native_NativeGenerator;
struct core_vec_Vec_codegen_native_NativeGenerator { codegen_native_NativeGenerator** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_native_NativeGenerator core_vec_Vec_codegen_native_NativeGenerator;
struct core_vec_Vec_codegen_native_NativeGenerator_ptr { codegen_native_NativeGenerator*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_native_NativeGenerator_ptr core_vec_Vec_codegen_native_NativeGenerator_ptr;
struct core_map_MapNode_str_codegen_native_NativeGenerator { char* key; codegen_native_NativeGenerator* value; struct core_map_MapNode_str_codegen_native_NativeGenerator* next; };
typedef struct core_map_MapNode_str_codegen_native_NativeGenerator core_map_MapNode_str_codegen_native_NativeGenerator;
struct core_map_Map_str_codegen_native_NativeGenerator { core_map_MapNode_str_codegen_native_NativeGenerator** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_native_NativeGenerator core_map_Map_str_codegen_native_NativeGenerator;
__attribute__((hot)) codegen_native_NativeGenerator** core_alloc_alloc_codegen_native_NativeGenerator(long long count);
__attribute__((hot)) codegen_native_NativeGenerator** core_alloc_resize_codegen_native_NativeGenerator(codegen_native_NativeGenerator** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_native_NativeGenerator(codegen_native_NativeGenerator** ptr);
__attribute__((hot)) codegen_native_NativeGenerator*** core_alloc_alloc_codegen_native_NativeGenerator_ptr(long long count);
__attribute__((hot)) codegen_native_NativeGenerator*** core_alloc_resize_codegen_native_NativeGenerator_ptr(codegen_native_NativeGenerator*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_native_NativeGenerator_ptr(codegen_native_NativeGenerator*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_native_NativeGenerator** core_alloc_alloc_core_map_MapNode_str_codegen_native_NativeGenerator(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_native_NativeGenerator(core_map_MapNode_str_codegen_native_NativeGenerator** ptr);

typedef GVal codegen_gpu_emit_GVal;
struct core_vec_Vec_codegen_gpu_emit_GVal { codegen_gpu_emit_GVal** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_gpu_emit_GVal core_vec_Vec_codegen_gpu_emit_GVal;
struct core_vec_Vec_codegen_gpu_emit_GVal_ptr { codegen_gpu_emit_GVal*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_gpu_emit_GVal_ptr core_vec_Vec_codegen_gpu_emit_GVal_ptr;
struct core_map_MapNode_str_codegen_gpu_emit_GVal { char* key; codegen_gpu_emit_GVal* value; struct core_map_MapNode_str_codegen_gpu_emit_GVal* next; };
typedef struct core_map_MapNode_str_codegen_gpu_emit_GVal core_map_MapNode_str_codegen_gpu_emit_GVal;
struct core_map_Map_str_codegen_gpu_emit_GVal { core_map_MapNode_str_codegen_gpu_emit_GVal** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_gpu_emit_GVal core_map_Map_str_codegen_gpu_emit_GVal;
__attribute__((hot)) codegen_gpu_emit_GVal** core_alloc_alloc_codegen_gpu_emit_GVal(long long count);
__attribute__((hot)) codegen_gpu_emit_GVal** core_alloc_resize_codegen_gpu_emit_GVal(codegen_gpu_emit_GVal** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_gpu_emit_GVal(codegen_gpu_emit_GVal** ptr);
__attribute__((hot)) codegen_gpu_emit_GVal*** core_alloc_alloc_codegen_gpu_emit_GVal_ptr(long long count);
__attribute__((hot)) codegen_gpu_emit_GVal*** core_alloc_resize_codegen_gpu_emit_GVal_ptr(codegen_gpu_emit_GVal*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_gpu_emit_GVal_ptr(codegen_gpu_emit_GVal*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_gpu_emit_GVal** core_alloc_alloc_core_map_MapNode_str_codegen_gpu_emit_GVal(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_gpu_emit_GVal(core_map_MapNode_str_codegen_gpu_emit_GVal** ptr);

typedef GpuEmitter codegen_gpu_emit_GpuEmitter;
struct core_vec_Vec_codegen_gpu_emit_GpuEmitter { codegen_gpu_emit_GpuEmitter** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_gpu_emit_GpuEmitter core_vec_Vec_codegen_gpu_emit_GpuEmitter;
struct core_vec_Vec_codegen_gpu_emit_GpuEmitter_ptr { codegen_gpu_emit_GpuEmitter*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_gpu_emit_GpuEmitter_ptr core_vec_Vec_codegen_gpu_emit_GpuEmitter_ptr;
struct core_map_MapNode_str_codegen_gpu_emit_GpuEmitter { char* key; codegen_gpu_emit_GpuEmitter* value; struct core_map_MapNode_str_codegen_gpu_emit_GpuEmitter* next; };
typedef struct core_map_MapNode_str_codegen_gpu_emit_GpuEmitter core_map_MapNode_str_codegen_gpu_emit_GpuEmitter;
struct core_map_Map_str_codegen_gpu_emit_GpuEmitter { core_map_MapNode_str_codegen_gpu_emit_GpuEmitter** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_gpu_emit_GpuEmitter core_map_Map_str_codegen_gpu_emit_GpuEmitter;
__attribute__((hot)) codegen_gpu_emit_GpuEmitter** core_alloc_alloc_codegen_gpu_emit_GpuEmitter(long long count);
__attribute__((hot)) codegen_gpu_emit_GpuEmitter** core_alloc_resize_codegen_gpu_emit_GpuEmitter(codegen_gpu_emit_GpuEmitter** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_gpu_emit_GpuEmitter(codegen_gpu_emit_GpuEmitter** ptr);
__attribute__((hot)) codegen_gpu_emit_GpuEmitter*** core_alloc_alloc_codegen_gpu_emit_GpuEmitter_ptr(long long count);
__attribute__((hot)) codegen_gpu_emit_GpuEmitter*** core_alloc_resize_codegen_gpu_emit_GpuEmitter_ptr(codegen_gpu_emit_GpuEmitter*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_gpu_emit_GpuEmitter_ptr(codegen_gpu_emit_GpuEmitter*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_gpu_emit_GpuEmitter** core_alloc_alloc_core_map_MapNode_str_codegen_gpu_emit_GpuEmitter(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_gpu_emit_GpuEmitter(core_map_MapNode_str_codegen_gpu_emit_GpuEmitter** ptr);

typedef GpuGenerator codegen_gpu_GpuGenerator;
struct core_vec_Vec_codegen_gpu_GpuGenerator { codegen_gpu_GpuGenerator** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_gpu_GpuGenerator core_vec_Vec_codegen_gpu_GpuGenerator;
struct core_vec_Vec_codegen_gpu_GpuGenerator_ptr { codegen_gpu_GpuGenerator*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_codegen_gpu_GpuGenerator_ptr core_vec_Vec_codegen_gpu_GpuGenerator_ptr;
struct core_map_MapNode_str_codegen_gpu_GpuGenerator { char* key; codegen_gpu_GpuGenerator* value; struct core_map_MapNode_str_codegen_gpu_GpuGenerator* next; };
typedef struct core_map_MapNode_str_codegen_gpu_GpuGenerator core_map_MapNode_str_codegen_gpu_GpuGenerator;
struct core_map_Map_str_codegen_gpu_GpuGenerator { core_map_MapNode_str_codegen_gpu_GpuGenerator** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_codegen_gpu_GpuGenerator core_map_Map_str_codegen_gpu_GpuGenerator;
__attribute__((hot)) codegen_gpu_GpuGenerator** core_alloc_alloc_codegen_gpu_GpuGenerator(long long count);
__attribute__((hot)) codegen_gpu_GpuGenerator** core_alloc_resize_codegen_gpu_GpuGenerator(codegen_gpu_GpuGenerator** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_gpu_GpuGenerator(codegen_gpu_GpuGenerator** ptr);
__attribute__((hot)) codegen_gpu_GpuGenerator*** core_alloc_alloc_codegen_gpu_GpuGenerator_ptr(long long count);
__attribute__((hot)) codegen_gpu_GpuGenerator*** core_alloc_resize_codegen_gpu_GpuGenerator_ptr(codegen_gpu_GpuGenerator*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_codegen_gpu_GpuGenerator_ptr(codegen_gpu_GpuGenerator*** ptr);
__attribute__((hot)) core_map_MapNode_str_codegen_gpu_GpuGenerator** core_alloc_alloc_core_map_MapNode_str_codegen_gpu_GpuGenerator(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_codegen_gpu_GpuGenerator(core_map_MapNode_str_codegen_gpu_GpuGenerator** ptr);

typedef MacroVal macros_MacroVal;
struct core_vec_Vec_macros_MacroVal { macros_MacroVal* data; long long len; long long capacity; };
typedef struct core_vec_Vec_macros_MacroVal core_vec_Vec_macros_MacroVal;
struct core_vec_Vec_macros_MacroVal_ptr { macros_MacroVal** data; long long len; long long capacity; };
typedef struct core_vec_Vec_macros_MacroVal_ptr core_vec_Vec_macros_MacroVal_ptr;
__attribute__((hot)) macros_MacroVal* core_alloc_alloc_macros_MacroVal(long long count);
__attribute__((hot)) macros_MacroVal* core_alloc_resize_macros_MacroVal(macros_MacroVal* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_macros_MacroVal(macros_MacroVal* ptr);
__attribute__((hot)) macros_MacroVal** core_alloc_alloc_macros_MacroVal_ptr(long long count);
__attribute__((hot)) macros_MacroVal** core_alloc_resize_macros_MacroVal_ptr(macros_MacroVal** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_macros_MacroVal_ptr(macros_MacroVal** ptr);

typedef MacroCtx macros_MacroCtx;
struct core_vec_Vec_macros_MacroCtx { macros_MacroCtx** data; long long len; long long capacity; };
typedef struct core_vec_Vec_macros_MacroCtx core_vec_Vec_macros_MacroCtx;
struct core_vec_Vec_macros_MacroCtx_ptr { macros_MacroCtx*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_macros_MacroCtx_ptr core_vec_Vec_macros_MacroCtx_ptr;
struct core_map_MapNode_str_macros_MacroCtx { char* key; macros_MacroCtx* value; struct core_map_MapNode_str_macros_MacroCtx* next; };
typedef struct core_map_MapNode_str_macros_MacroCtx core_map_MapNode_str_macros_MacroCtx;
struct core_map_Map_str_macros_MacroCtx { core_map_MapNode_str_macros_MacroCtx** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_macros_MacroCtx core_map_Map_str_macros_MacroCtx;
__attribute__((hot)) macros_MacroCtx** core_alloc_alloc_macros_MacroCtx(long long count);
__attribute__((hot)) macros_MacroCtx** core_alloc_resize_macros_MacroCtx(macros_MacroCtx** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_macros_MacroCtx(macros_MacroCtx** ptr);
__attribute__((hot)) macros_MacroCtx*** core_alloc_alloc_macros_MacroCtx_ptr(long long count);
__attribute__((hot)) macros_MacroCtx*** core_alloc_resize_macros_MacroCtx_ptr(macros_MacroCtx*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_macros_MacroCtx_ptr(macros_MacroCtx*** ptr);
__attribute__((hot)) core_map_MapNode_str_macros_MacroCtx** core_alloc_alloc_core_map_MapNode_str_macros_MacroCtx(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_macros_MacroCtx(core_map_MapNode_str_macros_MacroCtx** ptr);

typedef FnMacroExpander macros_FnMacroExpander;
struct core_vec_Vec_macros_FnMacroExpander { macros_FnMacroExpander** data; long long len; long long capacity; };
typedef struct core_vec_Vec_macros_FnMacroExpander core_vec_Vec_macros_FnMacroExpander;
struct core_vec_Vec_macros_FnMacroExpander_ptr { macros_FnMacroExpander*** data; long long len; long long capacity; };
typedef struct core_vec_Vec_macros_FnMacroExpander_ptr core_vec_Vec_macros_FnMacroExpander_ptr;
struct core_map_MapNode_str_macros_FnMacroExpander { char* key; macros_FnMacroExpander* value; struct core_map_MapNode_str_macros_FnMacroExpander* next; };
typedef struct core_map_MapNode_str_macros_FnMacroExpander core_map_MapNode_str_macros_FnMacroExpander;
struct core_map_Map_str_macros_FnMacroExpander { core_map_MapNode_str_macros_FnMacroExpander** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_macros_FnMacroExpander core_map_Map_str_macros_FnMacroExpander;
__attribute__((hot)) macros_FnMacroExpander** core_alloc_alloc_macros_FnMacroExpander(long long count);
__attribute__((hot)) macros_FnMacroExpander** core_alloc_resize_macros_FnMacroExpander(macros_FnMacroExpander** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_macros_FnMacroExpander(macros_FnMacroExpander** ptr);
__attribute__((hot)) macros_FnMacroExpander*** core_alloc_alloc_macros_FnMacroExpander_ptr(long long count);
__attribute__((hot)) macros_FnMacroExpander*** core_alloc_resize_macros_FnMacroExpander_ptr(macros_FnMacroExpander*** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_macros_FnMacroExpander_ptr(macros_FnMacroExpander*** ptr);
__attribute__((hot)) core_map_MapNode_str_macros_FnMacroExpander** core_alloc_alloc_core_map_MapNode_str_macros_FnMacroExpander(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_macros_FnMacroExpander(core_map_MapNode_str_macros_FnMacroExpander** ptr);

/* Primitive vec/map types for core modules */
struct core_vec_Vec_str { char** data; long long len; long long capacity; };
typedef struct core_vec_Vec_str core_vec_Vec_str;
__attribute__((hot)) char** core_alloc_alloc_str(long long count);
__attribute__((hot)) char** core_alloc_resize_str(char** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_str(char** ptr);
struct core_vec_Vec_i64 { long long* data; long long len; long long capacity; };
typedef struct core_vec_Vec_i64 core_vec_Vec_i64;
__attribute__((hot)) long long* core_alloc_alloc_i64(long long count);
__attribute__((hot)) long long* core_alloc_resize_i64(long long* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_i64(long long* ptr);
struct core_map_MapNode_str_bool { char* key; bool value; struct core_map_MapNode_str_bool* next; };
typedef struct core_map_MapNode_str_bool core_map_MapNode_str_bool;
struct core_map_Map_str_bool { core_map_MapNode_str_bool** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_bool core_map_Map_str_bool;
__attribute__((hot)) core_map_MapNode_str_bool** core_alloc_alloc_core_map_MapNode_str_bool(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_bool(core_map_MapNode_str_bool** ptr);
struct core_map_MapNode_str_str { char* key; char* value; struct core_map_MapNode_str_str* next; };
typedef struct core_map_MapNode_str_str core_map_MapNode_str_str;
struct core_map_Map_str_str { core_map_MapNode_str_str** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_str core_map_Map_str_str;
__attribute__((hot)) core_map_MapNode_str_str** core_alloc_alloc_core_map_MapNode_str_str(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_str(core_map_MapNode_str_str** ptr);
struct core_map_MapNode_str_i64 { char* key; long long value; struct core_map_MapNode_str_i64* next; };
typedef struct core_map_MapNode_str_i64 core_map_MapNode_str_i64;
struct core_map_Map_str_i64 { core_map_MapNode_str_i64** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_i64 core_map_Map_str_i64;
__attribute__((hot)) core_map_MapNode_str_i64** core_alloc_alloc_core_map_MapNode_str_i64(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_i64(core_map_MapNode_str_i64** ptr);
__attribute__((hot)) char* core_alloc_alloc_char(long long count);
__attribute__((hot)) void core_alloc_copy_char(char* dst, char* src, long long count);
__attribute__((hot)) char* core_alloc_resize_char(char* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_char(char* ptr);
