--- @meta

--- @class Clang
local clang = {}

--- @param exclude_declarations_from_pch boolean
--- @param display_diagnostics boolean
--- @return CXIndex
function clang.create_index(exclude_declarations_from_pch, display_diagnostics) end

--- @class CXIndex
local cx_index = {}

--- @param ast_file string
--- @return CXTranslationUnit|nil, string|nil err
function cx_index:load_translation_unit(ast_file) end

--- @param source_file_or_args string|string[]
--- @param args string[]|nil
--- @return CXTranslationUnit|nil, string|nil err
function cx_index:parse_translation_unit(source_file_or_args, args) end

--- @class CXTranslationUnit
local cx_translation_unit = {}

--- @return CXCursor|nil
function cx_translation_unit:cursor() end

--- @class CXCursor
local cx_cursor = {}

--- @return string
function cx_cursor:name() end

--- @alias CXCursorKindString string|"FunctionDecl"|"TypedefDecl"|"EnumDecl"|"EnumConstantDecl"|"StructDecl"|"UnionDecl"|"ClassDecl"|"FieldDecl"|"VarDecl"|"ParmDecl"|"ObjCInterfaceDecl"|"ObjCCategoryDecl"|"ObjCProtocolDecl"|"ObjCPropertyDecl"|"ObjCIvarDecl"|"ObjCInstanceMethodDecl"|"ObjCClassMethodDecl"|"ObjCImplementationDecl"|"ObjCCategoryImplDecl"|"CXXMethod"|"UnexposedDecl"|"ObjCSuperClassRef"|"ObjCProtocolRef"|"ObjCClassRef"|"TypeRef"|"TemplateRef"|"NamespaceRef"|"MemberRef"|"LabelRef"|"OverloadedDeclRef"|"VariableRef"|"IntegerLiteral"|"FixedPointLiteral"|"FloatingLiteral"|"ImaginaryLiteral"|"StringLiteral"|"CharacterLiteral"|"ParenExpr"|"UnaryOperator"|"ArraySubscriptExpr"|"ArraySectionExpr"|"OMPArrayShapingExpr"|"OMPIteratorExpr"|"BinaryOperator"|"CompoundAssignOperator"|"ConditionalOperator"|"CStyleCastExpr"|"CompoundLiteralExpr"|"InitListExpr"|"AddrLabelExpr"|"StmtExpr"|"GenericSelectionExpr"|"GNUNullExpr"|"CXXStaticCastExpr"|"CXXDynamicCastExpr"|"CXXReinterpretCastExpr"|"CXXConstCastExpr"|"CXXFunctionalCastExpr"|"CXXAddrspaceCastExpr"|"CXXTypeidExpr"|"CXXBoolLiteralExpr"|"CXXNullPtrLiteralExpr"|"CXXThisExpr"|"CXXThrowExpr"|"CXXNewExpr"|"CXXDeleteExpr"|"UnaryExpr"|"ObjCStringLiteral"|"ObjCBoolLiteralExpr"|"ObjCAvailabilityCheckExpr"|"ObjCSelfExpr"|"ObjCEncodeExpr"|"ObjCSelectorExpr"|"ObjCProtocolExpr"|"ObjCBridgedCastExpr"|"BlockExpr"|"PackExpansionExpr"|"SizeOfPackExpr"|"PackIndexingExpr"|"LambdaExpr"|"UnexposedExpr"|"DeclRefExpr"|"MemberRefExpr"|"CallExpr"|"ObjCMessageExpr"|"BuiltinBitCastExpr"|"ConceptSpecializationExpr"|"RequiresExpr"|"CXXParenListInitExpr"|"UnexposedStmt"|"DeclStmt"|"LabelStmt"|"CompoundStmt"|"CaseStmt"|"DefaultStmt"|"IfStmt"|"SwitchStmt"|"WhileStmt"|"DoStmt"|"ForStmt"|"GotoStmt"|"IndirectGotoStmt"|"ContinueStmt"|"BreakStmt"|"ReturnStmt"|"GCCAsmStmt"|"MSAsmStmt"|"ObjCAtTryStmt"|"ObjCAtCatchStmt"|"ObjCAtFinallyStmt"|"ObjCAtThrowStmt"|"ObjCAtSynchronizedStmt"|"ObjCAutoreleasePoolStmt"|"ObjCForCollectionStmt"|"CXXCatchStmt"|"CXXTryStmt"|"CXXForRangeStmt"|"SEHTryStmt"|"SEHExceptStmt"|"SEHFinallyStmt"|"SEHLeaveStmt"|"NullStmt"|"InvalidFile"|"InvalidCode"|"NoDeclFound"|"NotImplemented"|"TranslationUnit"|"UnexposedAttr"|"attribute(ibaction)"|"attribute(iboutlet)"|"attribute(iboutletcollection)"|"attribute(final)"|"attribute(override)"|"attribute(annotate)"|"asm label"|"attribute(packed)"|"attribute(pure)"|"attribute(const)"|"attribute(noduplicate)"|"attribute(constant)"|"attribute(device)"|"attribute(global)"|"attribute(host)"|"attribute(shared)"|"attribute(visibility)"|"attribute(dllexport)"|"attribute(dllimport)"|"attribute(ns_returns_retained)"|"attribute(ns_returns_not_retained)"|"attribute(ns_returns_autoreleased)"|"attribute(ns_consumes_self)"|"attribute(ns_consumed)"|"attribute(objc_exception)"|"attribute(NSObject)"|"attribute(objc_independent_class)"|"attribute(objc_precise_lifetime)"|"attribute(objc_returns_inner_pointer)"|"attribute(objc_requires_super)"|"attribute(objc_root_class)"|"attribute(objc_subclassing_restricted)"|"attribute(objc_protocol_requires_explicit_implementation)"|"attribute(objc_designated_initializer)"|"attribute(objc_runtime_visible)"|"attribute(objc_boxable)"|"attribute(flag_enum)"|"preprocessing directive"|"macro definition"|"macro expansion"|"inclusion directive"|"Namespace"|"LinkageSpec"|"C++ base class specifier"|"CXXConstructor"|"CXXDestructor"|"CXXConversion"|"TemplateTypeParameter"|"NonTypeTemplateParameter"|"TemplateTemplateParameter"|"FunctionTemplate"|"ClassTemplate"|"ClassTemplatePartialSpecialization"|"NamespaceAlias"|"UsingDirective"|"UsingDeclaration"|"TypeAliasDecl"|"ObjCSynthesizeDecl"|"ObjCDynamicDecl"|"CXXAccessSpecifier"|"ModuleImport"|"OMPCanonicalLoop"|"OMPMetaDirective"|"OMPParallelDirective"|"OMPSimdDirective"|"OMPTileDirective"|"OMPUnrollDirective"|"OMPReverseDirective"|"OMPInterchangeDirective"|"OMPForDirective"|"OMPForSimdDirective"|"OMPSectionsDirective"|"OMPSectionDirective"|"OMPScopeDirective"|"OMPSingleDirective"|"OMPMasterDirective"|"OMPCriticalDirective"|"OMPParallelForDirective"|"OMPParallelForSimdDirective"|"OMPParallelMasterDirective"|"OMPParallelMaskedDirective"|"OMPParallelSectionsDirective"|"OMPTaskDirective"|"OMPTaskyieldDirective"|"OMPBarrierDirective"|"OMPTaskwaitDirective"|"OMPAssumeDirective"|"OMPErrorDirective"|"OMPTaskgroupDirective"|"OMPFlushDirective"|"OMPDepobjDirective"|"OMPScanDirective"|"OMPOrderedDirective"|"OMPAtomicDirective"|"OMPTargetDirective"|"OMPTargetDataDirective"|"OMPTargetEnterDataDirective"|"OMPTargetExitDataDirective"|"OMPTargetParallelDirective"|"OMPTargetParallelForDirective"|"OMPTargetUpdateDirective"|"OMPTeamsDirective"|"OMPCancellationPointDirective"|"OMPCancelDirective"|"OMPTaskLoopDirective"|"OMPTaskLoopSimdDirective"|"OMPMasterTaskLoopDirective"|"OMPMaskedTaskLoopDirective"|"OMPMasterTaskLoopSimdDirective"|"OMPMaskedTaskLoopSimdDirective"|"OMPParallelMasterTaskLoopDirective"|"OMPParallelMaskedTaskLoopDirective"|"OMPParallelMasterTaskLoopSimdDirective"|"OMPParallelMaskedTaskLoopSimdDirective"|"OMPDistributeDirective"|"OMPDistributeParallelForDirective"|"OMPDistributeParallelForSimdDirective"|"OMPDistributeSimdDirective"|"OMPTargetParallelForSimdDirective"|"OMPTargetSimdDirective"|"OMPTeamsDistributeDirective"|"OMPTeamsDistributeSimdDirective"|"OMPTeamsDistributeParallelForSimdDirective"|"OMPTeamsDistributeParallelForDirective"|"OMPTargetTeamsDirective"|"OMPTargetTeamsDistributeDirective"|"OMPTargetTeamsDistributeParallelForDirective"|  "OMPTargetTeamsDistributeParallelForSimdDirective"|"OMPTargetTeamsDistributeSimdDirective"|"OMPInteropDirective"|"OMPDispatchDirective"|"OMPMaskedDirective"|"OMPGenericLoopDirective"|"OMPTeamsGenericLoopDirective"|"OMPTargetTeamsGenericLoopDirective"|"OMPParallelGenericLoopDirective"|"OMPTargetParallelGenericLoopDirective"|"OverloadCandidate"|"TypeAliasTemplateDecl"|"StaticAssert"|"FriendDecl"|"attribute(convergent)"|"attribute(warn_unused)"|"attribute(warn_unused_result)"|"attribute(aligned)"|"ConceptDecl"|"OpenACCComputeConstruct"|"OpenACCLoopConstruct"|"OpenACCCombinedConstruct"|"OpenACCDataConstruct"|"OpenACCEnterDataConstruct"|"OpenACCExitDataConstruct"|"OpenACCHostDataConstruct"|"OpenACCWaitConstruct"|"OpenACCInitConstruct"|"OpenACCShutdownConstruct"|"OpenACCSetConstruct"|"OpenACCUpdateConstruct"

--- @return CXCursorKindString
function cx_cursor:kind() end

--- @return CXCursor[]
function cx_cursor:children() end

--- @return string file_name, integer line_start, integer col_start, integer line_end, integer col_end
function cx_cursor:location() end

--- @return CXCursor[]
function cx_cursor:arguments() end

--- @return CXType|nil
function cx_cursor:type() end

--- @return CXCursor|nil
function cx_cursor:definition() end

--- @return boolean
function cx_cursor:is_definition() end

--- @return CXType|nil
function cx_cursor:get_underlying_type() end

--- @return boolean
function cx_cursor:is_static() end

--- @return boolean
function cx_cursor:is_extern_c() end

--- @return boolean
function cx_cursor:is_no_return() end

--- @return boolean
function cx_cursor:is_virtual() end

--- @return boolean
function cx_cursor:is_inlined() end

--- @return CXType|nil
function cx_cursor:result_type() end

--- @class CXType
local cx_type = {}

--- @return string
function cx_type:name() end

--- @return CXType|nil
function cx_type:result_type() end

--- @return CXType[]
function cx_type:arguments() end

--- @return CXCursor|nil
function cx_type:declaration() end

--- @return CXType|nil
function cx_type:pointee() end

--- @function boolean
function cx_type:is_pod() end

--- @function boolean
function cx_type:is_const() end

--- @function boolean
function cx_type:is_volatile() end

--- @function boolean
function cx_type:is_restrict() end

--- @function boolean
function cx_type:is_variadic() end

--- @return integer
function cx_type:get_array_size() end

--- @return CXType|nil
function cx_type:get_array_element_type() end

--- @return CXType|nil
function cx_type:get_named_type() end
