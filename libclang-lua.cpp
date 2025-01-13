#include <clang-c/CXFile.h>
#include <clang-c/Index.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/DeclObjC.h>
#include <clang/Frontend/ASTUnit.h>
#include <lua.hpp>

#define LCM_INDEX "ClangIndex"
#define LCM_TU "ClangTU"
#define LCM_CURSOR "ClangCursor"
#define LCM_TYPE "ClangType"

#define reg_func(X) { #X, l_##X }

static CXIndex *new_index(lua_State *L) {
    CXIndex *idx = (CXIndex *)lua_newuserdata(L, sizeof(CXIndex));
    luaL_getmetatable(L, LCM_INDEX);
    lua_setmetatable(L, -2);
    return idx;
}

static CXIndex get_index(lua_State *L, int n) {
    return *(CXIndex *)luaL_checkudata(L, n, LCM_INDEX);
}

static CXTranslationUnit *new_translation_unit(lua_State *L) {
    CXTranslationUnit *tu = (CXTranslationUnit *)lua_newuserdata(L, sizeof(CXTranslationUnit));
    luaL_getmetatable(L, LCM_TU);
    lua_setmetatable(L, -2);
    return tu;
}

static CXTranslationUnit get_translation_unit(lua_State *L, int n) {
    return *(CXTranslationUnit *)luaL_checkudata(L, n, LCM_TU);
}

static CXCursor *new_cursor(lua_State *L) {
    CXCursor *c = (CXCursor *)lua_newuserdata(L, sizeof(CXCursor));
    luaL_getmetatable(L, LCM_CURSOR);
    lua_setmetatable(L, -2);
    return c;
}

static CXCursor get_cursor(lua_State *L, int n) {
    return *(CXCursor *)luaL_checkudata(L, n, LCM_CURSOR);
}

static CXType *new_type(lua_State *L) {
    CXType *t = (CXType *)lua_newuserdata(L, sizeof(CXType));
    luaL_getmetatable(L, LCM_TYPE);
    lua_setmetatable(L, -2);
    return t;
}

static CXType get_type(lua_State *L, int n) {
    return *(CXType *)luaL_checkudata(L, n, LCM_TYPE);
}

static inline const clang::Decl *get_decl(CXCursor cur) {
    return static_cast<const clang::Decl *>(cur.data[0]);
}

// Index Functions

static int l_load_translation_unit(lua_State *L) {
    CXIndex idx = get_index(L, 1);
    const char *astFile = luaL_checkstring(L, 2);

    CXTranslationUnit *tu = new_translation_unit(L);
    *tu = clang_createTranslationUnit(idx, astFile);
    if (*tu == NULL) {
        lua_pushnil(L);
        lua_pushliteral(L, "Failed to open file: ");
        lua_pushvalue(L, 2);
        lua_concat(L, 2);
        return 2;
    }

    return 1;
}

static int l_parse_translation_unit(lua_State *L) {
    CXIndex idx = get_index(L, 1);
    int tabIndex;
    const char *sourceFile;

    if (lua_type(L, 2) == LUA_TTABLE) {
        sourceFile = NULL;
        tabIndex = 2;
    }
    else {
        sourceFile = luaL_checkstring(L, 2);
        luaL_checktype(L, 3, LUA_TTABLE);
        tabIndex = 3;
    }

    int nArgs = lua_rawlen(L, tabIndex);
    lua_checkstack(L, nArgs);
    char const **args = new char const *[nArgs];
    for (int i = 0; i < nArgs; i++) {
        lua_rawgeti(L, tabIndex, i + 1);
        args[i] = lua_tostring(L, -1);
    }

    CXTranslationUnit *tu = new_translation_unit(L);
    *tu = clang_parseTranslationUnit(idx, sourceFile, args, nArgs, NULL, 0, CXTranslationUnit_SkipFunctionBodies);
    delete[] args;

    if (*tu == NULL) {
        lua_pushnil(L);
        lua_pushliteral(L, "Failed to parse");

        return 2;
    }

    return 1;
}

static int l_index_gc(lua_State *L) {
    CXIndex idx = get_index(L, 1);
    clang_disposeIndex(idx);

    return 0;
}

static luaL_Reg index_functions[] = {
    reg_func(load_translation_unit),
    reg_func(parse_translation_unit),
    { "__gc", l_index_gc },
    { NULL,   NULL       },
};

// TranslationUnit Functions

static int l_cursor(lua_State *L) {
    CXTranslationUnit tu = get_translation_unit(L, 1);
    CXCursor *cur = new_cursor(L);

    *cur = clang_getTranslationUnitCursor(tu);
    if (clang_Cursor_isNull(*cur))
        lua_pushnil(L);
    return 1;
}

static int l_translation_unit_gc(lua_State *L) {
    CXTranslationUnit tu = get_translation_unit(L, 1);
    clang_disposeTranslationUnit(tu);

    return 0;
}

static luaL_Reg translation_unit_functions[]{
    reg_func(cursor),
    { "__gc", l_translation_unit_gc },
    { NULL,   NULL                  },
};

// Cursor Functions

static int l_name(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);

    CXString name = clang_getCursorSpelling(cur);
    lua_pushstring(L, clang_getCString(name));
    clang_disposeString(name);

    return 1;
}

static int l_kind(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    CXString kindName = clang_getCursorKindSpelling(clang_getCursorKind(cur));
    lua_pushstring(L, clang_getCString(kindName));
    clang_disposeString(kindName);

    return 1;
}

enum CXChildVisitResult lua_table_visitor(CXCursor cur, CXCursor par, CXClientData cd) {
    lua_State *L = (lua_State *)cd;
    CXCursor *c = new_cursor(L);
    *c = cur;
    int tablen = lua_rawlen(L, -2);
    lua_rawseti(L, -2, tablen + 1);

    return CXChildVisit_Continue;
}

static int l_children(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    lua_createtable(L, 0, 0);
    clang_visitChildren(cur, lua_table_visitor, L);

    return 1;
}

static int l_location(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    CXSourceRange range = clang_getCursorExtent(cur);

    CXFile file;
    unsigned int line, col;

    CXSourceLocation loc = clang_getRangeStart(range);
    clang_getSpellingLocation(loc, &file, &line, &col, NULL);
    CXString fname = clang_getFileName(file);
    lua_pushstring(L, clang_getCString(clang_getFileName(file)));
    clang_disposeString(fname);
    lua_pushinteger(L, line);
    lua_pushinteger(L, col);

    loc = clang_getRangeEnd(range);
    clang_getSpellingLocation(loc, &file, &line, &col, NULL);
    lua_pushinteger(L, line);
    lua_pushinteger(L, col);

    return 5;
}

static int l_arguments(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    uint nargs = clang_Cursor_getNumArguments(cur);

    lua_createtable(L, nargs, 0);
    for (uint i = 0; i < nargs; i++) {
        CXCursor *arg = new_cursor(L);
        *arg = clang_Cursor_getArgument(cur, i);
        lua_rawseti(L, -2, i + 1);
    }

    return 1;
}

static int l_type(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    CXType *type = new_type(L);
    *type = clang_getCursorType(cur);
    if (type->kind == CXType_Invalid)
        lua_pushnil(L);

    return 1;
}

static int l_definition(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    CXCursor *res = new_cursor(L);
    *res = clang_getCursorDefinition(cur);
    if (clang_Cursor_isNull(*res))
        lua_pushnil(L);

    return 1;
}

static int l_is_definition(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    lua_pushinteger(L, clang_isCursorDefinition(cur));

    return 1;
}

static int l_is_static(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    lua_pushboolean(L, clang_CXXMethod_isStatic(cur));

    return 1;
}

static int l_is_extern_c(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    const clang::Decl *d = get_decl(cur);

    if (const clang::FunctionDecl *fd = d->getAsFunction())
        lua_pushboolean(L, fd->isExternC());
    else
        lua_pushboolean(L, 0);

    return 1;
}

static int l_is_no_return(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    const clang::Decl *d = get_decl(cur);

    if (const clang::FunctionDecl *fd = d->getAsFunction())
        lua_pushboolean(L, fd->isNoReturn());
    else
        lua_pushboolean(L, 0);

    return 1;
}

static int l_is_virtual(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    lua_pushboolean(L, clang_CXXMethod_isVirtual(cur));

    return 1;
}

static int l_is_inlined(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    lua_pushboolean(L, clang_Cursor_isFunctionInlined(cur));

    return 1;
}

static int l_result_type(lua_State *L) {
    CXCursor cur = get_cursor(L, 1);
    CXType *type = new_type(L);

    *type = clang_getCursorResultType(cur);
    if (type->kind == CXType_Invalid)
        lua_pushnil(L);

    return 1;
}

// static int l_to_decl(lua_State *L) {
//     CXCursor cur = to_cursor(L, 1);

//     if (!clang_isDeclaration(cur.kind))
//         lua_pushnil(L);

//     const clang::Decl *d = new_decl(L);
//     d = static_cast<const clang::Decl *>(cur.data[0]);

//     return 1;
// }

static int l_cursor_equal(lua_State *L) {
    CXCursor cur1 = get_cursor(L, 1);
    CXCursor cur2 = get_cursor(L, 2);
    lua_pushboolean(L, clang_equalCursors(cur1, cur2));
    return 1;
}

static luaL_Reg cursor_functions[]{
    reg_func(name),
    { "__tostring", l_name         },
    reg_func(kind),
    reg_func(children),
    reg_func(location),
    reg_func(arguments),
    reg_func(type),
    reg_func(definition),
    reg_func(is_definition),
    reg_func(is_static),
    reg_func(is_extern_c),
    reg_func(is_no_return),
    reg_func(is_virtual),
    reg_func(is_inlined),
    reg_func(result_type),
    // reg_func(to_decl),
    { "__eq",       l_cursor_equal },
    { NULL,         NULL           },
};

// Type Functions

static int l_type_name(lua_State *L) {
    CXType type = get_type(L, 1);
    CXString str = clang_getTypeKindSpelling(type.kind);
    lua_pushstring(L, clang_getCString(str));
    clang_disposeString(str);

    return 1;
}

static int l_type_result_type(lua_State *L) {
    CXType type = get_type(L, 1);
    CXType *rt = new_type(L);

    *rt = clang_getResultType(type);
    if (rt->kind == CXType_Invalid)
        lua_pushnil(L);

    return 1;
}

static int l_type_arguments(lua_State *L) {
    CXType type = get_type(L, 1);
    unsigned int nArgs = clang_getNumArgTypes(type);

    lua_createtable(L, nArgs, 0);

    for (unsigned int i = 0; i < nArgs; i++) {
        CXType *arg = new_type(L);
        *arg = clang_getArgType(type, i);
        lua_rawseti(L, -2, i + 1);
    }

    return 1;
}

static int l_declaration(lua_State *L) {
    CXType type = get_type(L, 1);
    CXCursor *cur = new_cursor(L);
    *cur = clang_getTypeDeclaration(type);

    if (clang_Cursor_isNull(*cur))
        lua_pushnil(L);

    return 1;
}

static int l_pointee(lua_State *L) {
    CXType type = get_type(L, 1);
    CXType *res = new_type(L);

    *res = clang_getPointeeType(type);
    if (res->kind == CXType_Invalid)
        lua_pushnil(L);

    return 1;
}

static int l_is_pod(lua_State *L) {
    CXType type = get_type(L, 1);
    lua_pushboolean(L, clang_isPODType(type));

    return 1;
}

static int l_is_const(lua_State *L) {
    CXType type = get_type(L, 1);
    lua_pushboolean(L, clang_isConstQualifiedType(type));

    return 1;
}

static int l_is_volatile(lua_State *L) {
    CXType type = get_type(L, 1);
    lua_pushboolean(L, clang_isVolatileQualifiedType(type));

    return 1;
}

static int l_is_restrict(lua_State *L) {
    CXType type = get_type(L, 1);
    lua_pushboolean(L, clang_isRestrictQualifiedType(type));

    return 1;
}

static int l_is_variadic(lua_State *L) {
    CXType type = get_type(L, 1);

    int isVar = clang_isFunctionTypeVariadic(type);
    lua_pushboolean(L, isVar);

    return 1;
}

static int l_get_array_size(lua_State *L) {
    CXType type = get_type(L, 1);

    int num = clang_getArraySize(type);
    lua_pushinteger(L, num);

    return 1;
}

static int l_get_array_element_type(lua_State *L) {
    CXType type = get_type(L, 1);
    CXType *res = new_type(L);

    *res = clang_getArrayElementType(type);
    if (res->kind == CXType_Invalid)
        lua_pushnil(L);

    return 1;
}

static int l_get_named_type(lua_State *L) {
    CXType type = get_type(L, 1);
    CXType *res = new_type(L);

    *res = clang_Type_getNamedType(type);
    if (res == NULL)
        lua_pushnil(L);

    return 1;
}

static int l_type_equal(lua_State *L) {
    CXType type = get_type(L, 1);
    CXType type2 = get_type(L, 2);
    lua_pushboolean(L, clang_equalTypes(type, type2));

    return 1;
}

static luaL_Reg type_functions[]{
    { "name",        l_type_name        },
    { "__tostring",  l_type_name        },
    { "result_type", l_type_result_type },
    { "arguments",   l_type_arguments   },
    reg_func(is_pod),
    reg_func(declaration),
    reg_func(pointee),
    reg_func(is_const),
    reg_func(is_volatile),
    reg_func(is_restrict),
    reg_func(is_variadic),
    reg_func(get_array_size),
    reg_func(get_array_element_type),
    reg_func(get_named_type),
    { "__eq",        l_type_equal       },
    { NULL,          NULL               },
};

// Clang Functions

static int l_create_index(lua_State *L) {
    int excludePch = lua_toboolean(L, 1);
    int diagnostics = lua_toboolean(L, 2);
    CXIndex *idx = new_index(L);
    *idx = clang_createIndex(excludePch, diagnostics);
    return 1;
}

static luaL_Reg clang_functions[] = {
    reg_func(create_index),
    { NULL, NULL },
};

void new_metatable(lua_State *L, const char *name, luaL_Reg *reg) {
    luaL_newmetatable(L, name);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
}

extern "C" int luaopen_lua(lua_State *L) {
    new_metatable(L, LCM_INDEX, index_functions);
    new_metatable(L, LCM_TU, translation_unit_functions);
    new_metatable(L, LCM_CURSOR, cursor_functions);
    new_metatable(L, LCM_TYPE, type_functions);
    // new_metatable(L, LCM_DECL, decl_functions);

    lua_newtable(L);
    luaL_setfuncs(L, clang_functions, 0);
    return 1;
}
