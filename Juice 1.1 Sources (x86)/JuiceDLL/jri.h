/* -*- Mode: C; tab-width: 4; -*- */
/*******************************************************************************
 * Java Runtime Interface
 * Copyright (c) 1996 Netscape Communications Corporation. All rights reserved.
 ******************************************************************************/

#ifndef JRI_H
#define JRI_H

#include "jri_md.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Types
 ******************************************************************************/

typedef void*                       JRIRef;

typedef struct JRIGlobal*			JRIGlobalRef;

typedef struct JRINativeInterface	JRINativeInterface;

typedef const JRINativeInterface*	JRIEnv;

typedef struct JRIFieldThunk		JRIFieldThunk;

typedef struct JRIMethodThunk		JRIMethodThunk;

/* convenience types: */
typedef JRIRef		jref;
typedef JRIRef		jbooleanArray;
typedef JRIRef		jbyteArray;
typedef JRIRef		jcharArray;
typedef JRIRef		jshortArray;
typedef JRIRef		jintArray;
typedef JRIRef		jlongArray;
typedef JRIRef		jfloatArray;
typedef JRIRef		jdoubleArray;
typedef JRIRef		jobjectArray;
typedef JRIRef		jstringArray;
typedef JRIRef		jarrayArray;

typedef union JRIValue {
	jbool			z;
	jbyte			b;
	jchar			c;
	jshort			s;
	jint			i;
	jlong			l;
	jfloat			f;
	jdouble			d;
	jref			r;
} JRIValue;

typedef enum JRIMethodThunkType {
	JRIMethodThunkType_NameAndSig
} JRIMethodThunkType;

struct JRIMethodThunk {
	JRIMethodThunkType				type;
	void*							data0;
	void*							data1;
};

typedef enum JRIBoolean {
    JRIFalse		= 0,
    JRITrue			= 1
} JRIBoolean;

typedef struct JRINativeInfo {
    char*			nativeMethodName;	/* input */
    char*			nativeMethodSig;	/* input */
    void*			nativeMethodProc;	/* input */
} JRINativeInfo;

#define JRIConstructorMethodName	"<init>"

/*******************************************************************************
 * Signature Construction Macros
 ******************************************************************************/

/*
** These macros can be used to construct signature strings. Hopefully their names
** are a little easier to remember than the single character they correspond to.
** For example, to specify the signature of the method:
**
**	public int read(byte b[], int off, int len);
**
** you could write something like this in C:
**
**	char* readSig = JRISigMethod(JRISigArray(JRISigByte)
**								 JRISigInt
**								 JRISigInt) JRISigInt;
**
** Of course, don't put commas between the types.
*/
#define JRISigArray(T)		"[" T
#define JRISigByte			"B"
#define JRISigChar			"C"
#define JRISigClass(name)	"L" name ";"
#define JRISigFloat			"F"
#define JRISigDouble		"D"
#define JRISigMethod(args)	"(" args ")"
#define JRISigNoArgs		""
#define JRISigInt			"I"
#define JRISigLong			"J"
#define JRISigShort			"S"
#define JRISigVoid			"V"
#define JRISigBoolean		"Z"

/*******************************************************************************
 * Environments
 ******************************************************************************/

extern JRI_PUBLIC_API(JRIEnv*)
JRI_GetCurrentEnv(void);

/*
** LoadClass borrows the buffer that you hand to it, so it will copy it
** if it needs to. This is useful when defining a class from static class
** data.
*/
typedef jref
(*JRI_LoadClass_t)(JRIEnv* env, const char* buf, jsize bufLen);

/*
** Returns a jref to a class object from a fully qualified name (package
** names delimited by '/' and the class name). If the name begins with
** '[' (the array signature character), an array class is returned.
*/
typedef jref
(*JRI_FindClass_t)(JRIEnv* env, const char* name);

/* Working with Exceptions */

typedef void
(*JRI_Throw_t)(JRIEnv* env, jref obj);

typedef void
(*JRI_ThrowNew_t)(JRIEnv* env, jref clazz, const char* message);

typedef jref
(*JRI_ExceptionOccurred_t)(JRIEnv* env);

typedef void
(*JRI_ExceptionDescribe_t)(JRIEnv* env);

typedef void
(*JRI_ExceptionClear_t)(JRIEnv* env);

/*******************************************************************************
 * References
 ******************************************************************************/

typedef JRIGlobalRef
(*JRI_NewGlobalRef_t)(JRIEnv* env, jref ref);

typedef void
(*JRI_DisposeGlobalRef_t)(JRIEnv* env, JRIGlobalRef ref);

typedef jref
(*JRI_GetGlobalRef_t)(JRIEnv* env, JRIGlobalRef globalRef);

typedef void
(*JRI_SetGlobalRef_t)(JRIEnv* env, JRIGlobalRef globalRef, jref value);

typedef jbool
(*JRI_IsSameObject_t)(JRIEnv* env, jref r1, jref r2);

/*******************************************************************************
 * Object Operations
 ******************************************************************************/

typedef jref
(*JRI_NewObject_t)(JRIEnv* env, jref clazz, JRIMethodThunk* method, ...);

typedef jref
(*JRI_NewObjectA_t)(JRIEnv* env, jref clazz, JRIMethodThunk* method,
					JRIValue* args);

typedef jref
(*JRI_NewObjectV_t)(JRIEnv* env, jref clazz, JRIMethodThunk* method,
					va_list args);

typedef jref
(*JRI_GetObjectClass_t)(JRIEnv* env, jref obj);

typedef jbool
(*JRI_IsInstanceOf_t)(JRIEnv* env, jref obj, jref clazz);

/******************************************************************************/

/* Accessing Public Fields of Objects */

typedef jref
(*JRI_GetField_t)(JRIEnv* env, jref obj, JRIFieldThunk* field);

typedef jbool
(*JRI_GetField_boolean_t)(JRIEnv* env, jref obj, JRIFieldThunk* field);

typedef jbyte
(*JRI_GetField_byte_t)(JRIEnv* env, jref obj, JRIFieldThunk* field);

typedef jchar
(*JRI_GetField_char_t)(JRIEnv* env, jref obj, JRIFieldThunk* field);

typedef jshort
(*JRI_GetField_short_t)(JRIEnv* env, jref obj, JRIFieldThunk* field);

typedef jint
(*JRI_GetField_int_t)(JRIEnv* env, jref obj, JRIFieldThunk* field);

typedef jlong
(*JRI_GetField_long_t)(JRIEnv* env, jref obj, JRIFieldThunk* field);

typedef jfloat
(*JRI_GetField_float_t)(JRIEnv* env, jref obj, JRIFieldThunk* field);

typedef jdouble
(*JRI_GetField_double_t)(JRIEnv* env, jref obj, JRIFieldThunk* field);

/******************************************************************************/

typedef void
(*JRI_SetField_t)(JRIEnv* env, jref obj, JRIFieldThunk* field, jref value);

typedef void
(*JRI_SetField_boolean_t)(JRIEnv* env, jref obj, JRIFieldThunk* field, jbool value);

typedef void
(*JRI_SetField_byte_t)(JRIEnv* env, jref obj, JRIFieldThunk* field, jbyte value);

typedef void
(*JRI_SetField_char_t)(JRIEnv* env, jref obj, JRIFieldThunk* field, jchar value);

typedef void
(*JRI_SetField_short_t)(JRIEnv* env, jref obj, JRIFieldThunk* field, jshort value);

typedef void
(*JRI_SetField_int_t)(JRIEnv* env, jref obj, JRIFieldThunk* field, jint value);

typedef void
(*JRI_SetField_long_t)(JRIEnv* env, jref obj, JRIFieldThunk* field, jlong value);

typedef void
(*JRI_SetField_float_t)(JRIEnv* env, jref obj, JRIFieldThunk* field, jfloat value);

typedef void
(*JRI_SetField_double_t)(JRIEnv* env, jref obj, JRIFieldThunk* field, jdouble value);

/******************************************************************************/

/* Calling Public Dynamic Methods of Objects */

typedef JRIValue
(*JRI_CallMethod_t)(JRIEnv* env, jref obj, JRIMethodThunk* method, ...);

typedef JRIValue
(*JRI_ApplyMethodV_t)(JRIEnv* env, jref obj, JRIMethodThunk* method, va_list args);

typedef JRIValue
(*JRI_ApplyMethod_t)(JRIEnv* env, jref obj, JRIMethodThunk* method,
			   JRIValue* valueArray);

/*******************************************************************************
 * Class Operations
 ******************************************************************************/

/*
** Determines whether the first class is a subclass of the second, or
** whether it has the second class as one of its interfaces.
*/
typedef jbool
(*JRI_IsSubclassOf_t)(JRIEnv* env, jref clazz, jref super);

/******************************************************************************/

/* Accessing Public Static Fields of Objects */

typedef jref
(*JRI_GetStaticField_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field);

typedef jbool
(*JRI_GetStaticField_boolean_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field);

typedef jbyte
(*JRI_GetStaticField_byte_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field);

typedef jchar
(*JRI_GetStaticField_char_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field);

typedef jshort
(*JRI_GetStaticField_short_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field);

typedef jint
(*JRI_GetStaticField_int_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field);

typedef jlong
(*JRI_GetStaticField_long_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field);

typedef jfloat
(*JRI_GetStaticField_float_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field);

typedef jdouble
(*JRI_GetStaticField_double_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field);

/******************************************************************************/

typedef void
(*JRI_SetStaticField_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field,
						jref value);

typedef void
(*JRI_SetStaticField_boolean_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field,
						    jbool value);

typedef void
(*JRI_SetStaticField_byte_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field,
						 jbyte value);

typedef void
(*JRI_SetStaticField_char_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field,
						 jchar value);

typedef void
(*JRI_SetStaticField_short_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field,
						  jshort value);

typedef void
(*JRI_SetStaticField_int_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field,
				        jint value);

typedef void
(*JRI_SetStaticField_long_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field,
						 jlong value);

typedef void
(*JRI_SetStaticField_float_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field,
						  jfloat value);

typedef void
(*JRI_SetStaticField_double_t)(JRIEnv* env, jref clazz, JRIFieldThunk* field,
						   jdouble value);

/******************************************************************************/

/* Calling Public Static Methods of Objects */

typedef JRIValue
(*JRI_CallStaticMethod_t)(JRIEnv* env, jref clazz, JRIMethodThunk* method, ...);

typedef JRIValue
(*JRI_ApplyStaticMethodV_t)(JRIEnv* env, jref clazz, JRIMethodThunk* method,
					 va_list args);

typedef JRIValue
(*JRI_ApplyStaticMethod_t)(JRIEnv* env, jref clazz, JRIMethodThunk* method,
					 JRIValue* valueArray);

/*******************************************************************************
 * String Operations
 ******************************************************************************/

/* Unicode Interface */

typedef jref
(*JRI_NewString_t)(JRIEnv* env, const jchar* bytes, jsize length);

typedef jsize
(*JRI_GetStringLength_t)(JRIEnv* env, jref string);

/* This returns a const jchar* pointer directly into the string's contents: */
typedef const jchar*
(*JRI_GetStringChars_t)(JRIEnv* env, jref string);

/* Mallocs a C string that must be freed by caller: */
typedef jchar*
(*JRI_CopyString_t)(JRIEnv* env, jref string);

/* UTF Interface */

typedef jref
(*JRI_NewStringUTF_t)(JRIEnv* env, const char* bytes, jsize length);

typedef jsize
(*JRI_GetStringUTFLength_t)(JRIEnv* env, jref string);

/* Mallocs a UTF C string that must be freed by caller: */
typedef char*
(*JRI_CopyStringUTF_t)(JRIEnv* env, jref string);

/*******************************************************************************
 * Scalar Array Operations
 ******************************************************************************/

typedef jref
(*JRI_NewScalarArray_t)(JRIEnv* env, jsize length, const jbyte* bytes);

typedef jsize
(*JRI_GetScalarArrayLength_t)(JRIEnv* env, jref array);

typedef jbyte*
(*JRI_GetScalarArrayData_t)(JRIEnv* env, jref array);

/*******************************************************************************
 * Object Array Operations
 ******************************************************************************/

typedef jref
(*JRI_NewObjectArray_t)(JRIEnv* env, jsize length, jref elementClass,
						jref initialElement);

typedef jsize
(*JRI_GetObjectArrayLength_t)(JRIEnv* env, jref array);

typedef jref
(*JRI_GetObjectArrayElement_t)(JRIEnv* env, jref array, jsize index);

typedef void
(*JRI_SetObjectArrayElement_t)(JRIEnv* env, jref array, jsize index,
							   jref value);

/*******************************************************************************
 * Native Bootstrap
 ******************************************************************************/

/*
** This routine is used by the generated stub code to look up field
** positions and method identifiers for native access and invocation.
*/
typedef void
(*JRI_RegisterNatives_t)(JRIEnv* env, jref clazz,
						 JRINativeInfo* nativeInfoArray);

typedef void
(*JRI_UnregisterNatives_t)(JRIEnv* env, jref clazz);

/******************************************************************************/

struct JRIFieldThunk {
	void*			data;
	JRI_GetField_t	get;	/* cast this to the right accessor type before calling */
	JRI_SetField_t	set;	/* cast this to the right accessor type before calling */
};

struct JRINativeInterface {
    void*							Reserved0;
    void*							Reserved1;
    void*							Reserved2;
	JRI_LoadClass_t					LoadClass;
	JRI_FindClass_t					FindClass;
	JRI_Throw_t						Throw;
	JRI_ThrowNew_t					ThrowNew;
	JRI_ExceptionOccurred_t			ExceptionOccurred;
	JRI_ExceptionDescribe_t			ExceptionDescribe;
	JRI_ExceptionClear_t			ExceptionClear;
	JRI_NewGlobalRef_t				NewGlobalRef;
	JRI_DisposeGlobalRef_t			DisposeGlobalRef;
	JRI_GetGlobalRef_t				GetGlobalRef;
	JRI_SetGlobalRef_t				SetGlobalRef;
	JRI_IsSameObject_t				IsSameObject;
	JRI_NewObject_t					NewObject;
	JRI_NewObjectA_t				NewObjectA;
	JRI_NewObjectV_t				NewObjectV;
	JRI_GetObjectClass_t			GetObjectClass;
	JRI_IsInstanceOf_t				IsInstanceOf;
	JRI_GetField_t					GetField;
	JRI_GetField_boolean_t			GetField_boolean;
	JRI_GetField_byte_t				GetField_byte;
	JRI_GetField_char_t				GetField_char;
	JRI_GetField_short_t			GetField_short;
	JRI_GetField_int_t				GetField_int;
	JRI_GetField_long_t				GetField_long;
	JRI_GetField_float_t			GetField_float;
	JRI_GetField_double_t			GetField_double;
	JRI_SetField_t					SetField;
	JRI_SetField_boolean_t			SetField_boolean;
	JRI_SetField_byte_t				SetField_byte;
	JRI_SetField_char_t				SetField_char;
	JRI_SetField_short_t			SetField_short;
	JRI_SetField_int_t				SetField_int;
	JRI_SetField_long_t				SetField_long;
	JRI_SetField_float_t			SetField_float;
	JRI_SetField_double_t			SetField_double;
	JRI_CallMethod_t				CallMethod;
	JRI_ApplyMethodV_t				ApplyMethodV;
	JRI_ApplyMethod_t				ApplyMethod;
	JRI_IsSubclassOf_t				IsSubclassOf;
	JRI_GetStaticField_t			GetStaticField;
	JRI_GetStaticField_boolean_t	GetStaticField_boolean;
	JRI_GetStaticField_byte_t		GetStaticField_byte;
	JRI_GetStaticField_char_t		GetStaticField_char;
	JRI_GetStaticField_short_t		GetStaticField_short;
	JRI_GetStaticField_int_t		GetStaticField_int;
	JRI_GetStaticField_long_t		GetStaticField_long;
	JRI_GetStaticField_float_t		GetStaticField_float;
	JRI_GetStaticField_double_t		GetStaticField_double;
	JRI_SetStaticField_t			SetStaticField;
	JRI_SetStaticField_boolean_t	SetStaticField_boolean;
	JRI_SetStaticField_byte_t		SetStaticField_byte;
	JRI_SetStaticField_char_t		SetStaticField_char;
	JRI_SetStaticField_short_t		SetStaticField_short;
	JRI_SetStaticField_int_t		SetStaticField_int;
	JRI_SetStaticField_long_t		SetStaticField_long;
	JRI_SetStaticField_float_t		SetStaticField_float;
	JRI_SetStaticField_double_t		SetStaticField_double;
	JRI_CallStaticMethod_t			CallStaticMethod;
	JRI_ApplyStaticMethodV_t		ApplyStaticMethodV;
	JRI_ApplyStaticMethod_t			ApplyStaticMethod;
	JRI_NewString_t					NewString;
	JRI_GetStringLength_t			GetStringLength;
	JRI_GetStringChars_t			GetStringChars;
	JRI_CopyString_t				CopyString;
	JRI_NewStringUTF_t				NewStringUTF;
	JRI_GetStringUTFLength_t		GetStringUTFLength;
	JRI_CopyStringUTF_t				CopyStringUTF;
	JRI_NewScalarArray_t			NewScalarArray;
	JRI_GetScalarArrayLength_t		GetScalarArrayLength;
	JRI_GetScalarArrayData_t		GetScalarArrayData;
	JRI_NewObjectArray_t			NewObjectArray;
	JRI_GetObjectArrayLength_t		GetObjectArrayLength;
	JRI_GetObjectArrayElement_t		GetObjectArrayElement;
	JRI_SetObjectArrayElement_t		SetObjectArrayElement;
	JRI_RegisterNatives_t			RegisterNatives;
	JRI_UnregisterNatives_t			UnregisterNatives;
};

/******************************************************************************/

/*
** LoadClass borrows the buffer that you hand to it, so it will Copy it
** if it needs to. This is useful when defining a class from static class
** data.
*/
#define JRI_LoadClass(env, buf, bufLen)		\
	(((*(env))->LoadClass)(env, buf, bufLen))

/*
** Returns a jref to a class object from a fully qualified name (package
** names delimited by '/' and the class name). If the name begins with
** '[' (the array signature character), an array class is returned.
*/
#define JRI_FindClass(env, name)		\
	(((*(env))->FindClass)(env, name))

/* Working with Exceptions */

#define JRI_Throw(env, obj)	\
	(((*(env))->Throw)(env, obj))

#define JRI_ThrowNew(env, clazz, message)		\
	(((*(env))->ThrowNew)(env, clazz, message))

#define JRI_ExceptionOccurred(env)		\
	(((*(env))->ExceptionOccurred)(env))

#define JRI_ExceptionDescribe(env)		\
	(((*(env))->ExceptionDescribe)(env))

#define JRI_ExceptionClear(env)		\
	(((*(env))->ExceptionClear)(env))

/*******************************************************************************
 * References
 ******************************************************************************/

#define JRI_NewGlobalRef(env, ref)		\
	(((*(env))->NewGlobalRef)(env, ref))

#define JRI_DisposeGlobalRef(env, ref)	\
	(((*(env))->DisposeGlobalRef)(env, ref))

#define JRI_GetGlobalRef(env, globalRef)		\
	(((*(env))->GetGlobalRef)(env, globalRef))

#define JRI_SetGlobalRef(env, globalRef, value)	\
	(((*(env))->SetGlobalRef)(env, globalRef, value))

#define JRI_IsSameObject(env, r1, r2)		\
	(((*(env))->IsSameObject)(env, r1, r2))

/*******************************************************************************
 * Object Operations
 ******************************************************************************/

#define JRI_NewObjectM(env)		((*(env))->NewObject)
#define JRI_NewObject			JRI_NewObjectM(env)	/* hard coded 'env' */

#define JRI_NewObjectA(env, clazz, method, args)		\
	(((*(env))->NewObjectA)(env, clazz, method, args))

#define JRI_NewObjectV(env, clazz, method, args)		\
	(((*(env))->NewObjectV)(env, clazz, method, args))

#define JRI_GetObjectClass(env, obj)		\
	(((*(env))->GetObjectClass)(env, obj))

#define JRI_IsInstanceOf(env, obj, clazz)	\
	(((*(env))->IsInstanceOf)(env, obj, clazz))

/******************************************************************************/

/* Accessing Public Fields of Objects */

#define JRI_GetField(env, obj, fieldThunk)	\
	(((JRI_GetField_t)(fieldThunk)->get)(env, obj, fieldThunk))

#define JRI_GetField_boolean(env, obj, fieldThunk)	\
	(((JRI_GetField_boolean_t)(fieldThunk)->get)(env, obj, fieldThunk))

#define JRI_GetField_byte(env, obj, fieldThunk)	\
	(((JRI_GetField_byte_t)(fieldThunk)->get)(env, obj, fieldThunk))

#define JRI_GetField_char(env, obj, fieldThunk)	\
	(((JRI_GetField_char_t)(fieldThunk)->get)(env, obj, fieldThunk))

#define JRI_GetField_short(env, obj, fieldThunk)	\
	(((JRI_GetField_short_t)(fieldThunk)->get)(env, obj, fieldThunk))

#define JRI_GetField_int(env, obj, fieldThunk)	\
	(((JRI_GetField_int_t)(fieldThunk)->get)(env, obj, fieldThunk))

#define JRI_GetField_long(env, obj, fieldThunk)	\
	(((JRI_GetField_long_t)(fieldThunk)->get)(env, obj, fieldThunk))

#define JRI_GetField_float(env, obj, fieldThunk)	\
	(((JRI_GetField_float_t)(fieldThunk)->get)(env, obj, fieldThunk))

#define JRI_GetField_double(env, obj, fieldThunk)	\
	(((JRI_GetField_double_t)(fieldThunk)->get)(env, obj, fieldThunk))

/******************************************************************************/

#define JRI_SetField(env, obj, fieldThunk, value)	\
	(((JRI_SetField_t)(fieldThunk)->set)(env, obj, fieldThunk, value))

#define JRI_SetField_boolean(env, obj, fieldThunk, value)	\
	(((JRI_SetField_boolean_t)(fieldThunk)->set)(env, obj, fieldThunk, value))

#define JRI_SetField_byte(env, obj, fieldThunk, value)	\
	(((JRI_SetField_byte_t)(fieldThunk)->set)(env, obj, fieldThunk, value))

#define JRI_SetField_char(env, obj, fieldThunk, value)	\
	(((JRI_SetField_char_t)(fieldThunk)->set)(env, obj, fieldThunk, value))

#define JRI_SetField_short(env, obj, fieldThunk, value)	\
	(((JRI_SetField_short_t)(fieldThunk)->set)(env, obj, fieldThunk, value))

#define JRI_SetField_int(env, obj, fieldThunk, value)	\
	(((JRI_SetField_int_t)(fieldThunk)->set)(env, obj, fieldThunk, value))

#define JRI_SetField_long(env, obj, fieldThunk, value)	\
	(((JRI_SetField_long_t)(fieldThunk)->set)(env, obj, fieldThunk, value))

#define JRI_SetField_float(env, obj, fieldThunk, value)	\
	(((JRI_SetField_float_t)(fieldThunk)->set)(env, obj, fieldThunk, value))

#define JRI_SetField_double(env, obj, fieldThunk, value)	\
	(((JRI_SetField_double_t)(fieldThunk)->set)(env, obj, fieldThunk, value))

/******************************************************************************/

/* Calling Public Dynamic Methods of Objects */

#define JRI_CallMethodM(env)			((*(env))->CallMethod)
#define JRI_CallMethod					JRI_CallMethodM(env)	/* hard coded 'env' */

#define JRI_ApplyMethodV(env, obj, methodThunk, args)	\
	(((*(env))->ApplyMethodV)(env, obj, methodThunk, args))

#define JRI_ApplyMethod(env, obj, methodThunk, args)	\
	(((*(env))->ApplyMethod)(env, obj, methodThunk, args))

/*******************************************************************************
 * Class Operations
 ******************************************************************************/

/*
** Determines whether the first class is a subclass of the second, or
** whether it has the second class as one of its interfaces.
*/
#define JRI_IsSubclassOf(env, clazz, super)	\
	(((*(env))->IsSubclassOf)(env, clazz, super))

/******************************************************************************/

/* Accessing Public Static Fields of Objects */

#define JRI_GetStaticField(env, clazz, fieldThunk)	\
	(((JRI_GetStaticField_t)(fieldThunk)->get)(env, clazz, fieldThunk))

#define JRI_GetStaticField_boolean(env, clazz, fieldThunk)	\
	(((JRI_GetStaticField_boolean_t)(fieldThunk)->get)(env, clazz, fieldThunk))

#define JRI_GetStaticField_byte(env, clazz, fieldThunk)	\
	(((JRI_GetStaticField_byte_t)(fieldThunk)->get)(env, clazz, fieldThunk))

#define JRI_GetStaticField_char(env, clazz, fieldThunk)	\
	(((JRI_GetStaticField_char_t)(fieldThunk)->get)(env, clazz, fieldThunk))

#define JRI_GetStaticField_short(env, clazz, fieldThunk)	\
	(((JRI_GetStaticField_short_t)(fieldThunk)->get)(env, clazz, fieldThunk))

#define JRI_GetStaticField_int(env, clazz, fieldThunk)	\
	(((JRI_GetStaticField_int_t)(fieldThunk)->get)(env, clazz, fieldThunk))

#define JRI_GetStaticField_long(env, clazz, fieldThunk)	\
	(((JRI_GetStaticField_long_t)(fieldThunk)->get)(env, clazz, fieldThunk))

#define JRI_GetStaticField_float(env, clazz, fieldThunk)	\
	(((JRI_GetStaticField_float_t)(fieldThunk)->get)(env, clazz, fieldThunk))

#define JRI_GetStaticField_double(env, clazz, fieldThunk)	\
	(((JRI_GetStaticField_double_t)(fieldThunk)->get)(env, clazz, fieldThunk))

/******************************************************************************/

#define JRI_SetStaticField(env, clazz, fieldThunk, value)	\
	(((JRI_SetStaticField_t)(fieldThunk)->set)(env, clazz, fieldThunk, value))

#define JRI_SetStaticField_boolean(env, clazz, fieldThunk, value)	\
	(((JRI_SetStaticField_boolean_t)(fieldThunk)->set)(env, clazz, fieldThunk, value))

#define JRI_SetStaticField_byte(env, clazz, fieldThunk, value)	\
	(((JRI_SetStaticField_byte_t)(fieldThunk)->set)(env, clazz, fieldThunk, value))

#define JRI_SetStaticField_char(env, clazz, fieldThunk, value)	\
	(((JRI_SetStaticField_char_t)(fieldThunk)->set)(env, clazz, fieldThunk, value))

#define JRI_SetStaticField_short(env, clazz, fieldThunk, value)	\
	(((JRI_SetStaticField_short_t)(fieldThunk)->set)(env, clazz, fieldThunk, value))

#define JRI_SetStaticField_int(env, clazz, fieldThunk, value)	\
	(((JRI_SetStaticField_int_t)(fieldThunk)->set)(env, clazz, fieldThunk, value))

#define JRI_SetStaticField_long(env, clazz, fieldThunk, value)	\
	(((JRI_SetStaticField_long_t)(fieldThunk)->set)(env, clazz, fieldThunk, value))

#define JRI_SetStaticField_float(env, clazz, fieldThunk, value)	\
	(((JRI_SetStaticField_float_t)(fieldThunk)->set)(env, clazz, fieldThunk, value))

#define JRI_SetStaticField_double(env, clazz, fieldThunk, value)	\
	(((JRI_SetStaticField_double_t)(fieldThunk)->set)(env, clazz, fieldThunk, value))

/******************************************************************************/

/* Calling Public Static Methods of Objects */

#define JRI_CallStaticMethodM(env)			((*(env))->CallStaticMethod)
#define JRI_CallStaticMethod				JRI_CallStaticMethodM(env)	/* hard coded 'env' */

#define JRI_ApplyStaticMethodV(env, clazz, method, args)		\
	(((*(env))->ApplyStaticMethodV)(env, clazz, method, args))

#define JRI_ApplyStaticMethod(env, clazz, method, args)		\
	(((*(env))->ApplyStaticMethod)(env, clazz, method, args))

/*******************************************************************************
 * String Operations
 ******************************************************************************/

/* Unicode Interface */

#define JRI_NewString(env, bytes, length)	\
	(((*(env))->NewString)(env, bytes, length))

#define JRI_GetStringLength(env, string)	\
	(((*(env))->GetStringLength)(env, string))

/* This returns a const jchar* pointer directly into the string's contents: */
#define JRI_GetStringChars(env, string)		\
	(((*(env))->GetStringChars)(env, string))

/* Mallocs a C string that must be freed by caller: */
#define JRI_CopyString(env, string)		\
	(((*(env))->CopyString)(env, string))

/* UTF Interface */

#define JRI_NewStringUTF(env, bytes, length)		\
	(((*(env))->NewStringUTF)(env, bytes, length))

#define JRI_GetStringUTFLength(env, string)		\
	(((*(env))->GetStringUTFLength)(env, string))

/* Mallocs a UTF C string that must be freed by caller: */
#define JRI_CopyStringUTF(env, string)	\
	(((*(env))->CopyStringUTF)(env, string))

/*******************************************************************************
 * Scalar Array Operations
 ******************************************************************************/

#define JRI_NewScalarArray(env, length, bytes)		\
	(((*(env))->NewScalarArray)(env, length, bytes))

#define JRI_GetScalarArrayLength(env, array)		\
	(((*(env))->GetScalarArrayLength)(env, array))

#define JRI_GetScalarArrayData(env, array)	\
	(((*(env))->GetScalarArrayData)(env, array))

/*******************************************************************************
 * Specific Scalar Array Types
 ******************************************************************************/

#define JRI_NewBooleanArray(env, length, initialValues)	\
	JRI_NewScalarArray(env, length, (jbyte*)(initialValues))
#define JRI_GetBooleanArrayLength(env, array)	\
	JRI_GetScalarArrayLength(env, array)
#define JRI_GetBooleanArrayData(env, array)	\
	((jbool*)JRI_GetScalarArrayData(env, array))

#define JRI_NewByteArray(env, length, initialValues)	\
	JRI_NewScalarArray(env, length, (jbyte*)(initialValues))
#define JRI_GetByteArrayLength(env, array)	\
	JRI_GetScalarArrayLength(env, array)
#define JRI_GetByteArrayData(env, array)	\
	JRI_GetScalarArrayData(env, array)

#define JRI_NewCharArray(env, length, initialValues)	\
	JRI_NewScalarArray(env, ((length) * sizeof(jchar)), (jbyte*)(initialValues))
#define JRI_GetCharArrayLength(env, array)	\
	JRI_GetScalarArrayLength(env, array)
#define JRI_GetCharArrayData(env, array)		   \
	((jchar*)JRI_GetScalarArrayData(env, array))

#define JRI_NewShortArray(env, length, initialValues)	\
	JRI_NewScalarArray(env, ((length) * sizeof(jshort)), (jbyte*)(initialValues))
#define JRI_GetShortArrayLength(env, array)	\
	JRI_GetScalarArrayLength(env, array)
#define JRI_GetShortArrayData(env, array)		   \
	((jshort*)JRI_GetScalarArrayData(env, array))

#define JRI_NewIntArray(env, length, initialValues)	\
	JRI_NewScalarArray(env, ((length) * sizeof(jint)), (jbyte*)(initialValues))
#define JRI_GetIntArrayLength(env, array)	\
	JRI_GetScalarArrayLength(env, array)
#define JRI_GetIntArrayData(env, array)		   \
	((jint*)JRI_GetScalarArrayData(env, array))

#define JRI_NewLongArray(env, length, initialValues)	\
	JRI_NewScalarArray(env, ((length) * sizeof(jlong)), (jbyte*)(initialValues))
#define JRI_GetLongArrayLength(env, array)	\
	JRI_GetScalarArrayLength(env, array)
#define JRI_GetLongArrayData(env, array)		   \
	((jlong*)JRI_GetScalarArrayData(env, array))

#define JRI_NewFloatArray(env, length, initialValues)	\
	JRI_NewScalarArray(env, ((length) * sizeof(jfloat)), (jbyte*)(initialValues))
#define JRI_GetFloatArrayLength(env, array)	\
	JRI_GetScalarArrayLength(env, array)
#define JRI_GetFloatArrayData(env, array)		   \
	((jfloat*)JRI_GetScalarArrayData(env, array))

#define JRI_NewDoubleArray(env, length, initialValues)	\
	JRI_NewScalarArray(env, ((length) * sizeof(jdouble)), (jbyte*)(initialValues))
#define JRI_GetDoubleArrayLength(env, array)	\
	JRI_GetScalarArrayLength(env, array)
#define JRI_GetDoubleArrayData(env, array)		   \
	((jdouble*)JRI_GetScalarArrayData(env, array))

/*******************************************************************************
 * Object Array Operations
 ******************************************************************************/

#define JRI_NewObjectArray(env, length, elementClass, initialElement)		\
	(((*(env))->NewObjectArray)(env, length, elementClass, initialElement))

#define JRI_GetObjectArrayLength(env, array)		\
	(((*(env))->GetObjectArrayLength)(env, array))

#define JRI_GetObjectArrayElement(env, array, index)	\
	(((*(env))->GetObjectArrayElement)(env, array, index))

#define JRI_SetObjectArrayElement(env, array, index, value)		\
	(((*(env))->SetObjectArrayElement)(env, array, index, value))

/*******************************************************************************
 * Native Bootstrap
 ******************************************************************************/

/*
** This routine is used by the generated stub code to look up field
** positions and method identifiers for native access and invocation.
*/
#define JRI_RegisterNatives(env, clazz, nativeInfoArray)		\
	(((*(env))->RegisterNatives)(env, clazz, nativeInfoArray))

#define JRI_UnregisterNatives(env, clazz)		\
	(((*(env))->UnregisterNatives)(env, clazz))

/*******************************************************************************
 * Glue Routines -- These are used to auto-initialize fields
 ******************************************************************************/

JRI_PUBLIC_API(jref)
JRI_GetFieldByName(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jbool)
JRI_GetFieldByName_boolean(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jbyte)
JRI_GetFieldByName_byte(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jchar)
JRI_GetFieldByName_char(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jshort)
JRI_GetFieldByName_short(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jint)
JRI_GetFieldByName_int(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jlong)
JRI_GetFieldByName_long(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jfloat)
JRI_GetFieldByName_float(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jdouble)
JRI_GetFieldByName_double(JRIEnv* env, jref obj, JRIFieldThunk* field);

/******************************************************************************/

JRI_PUBLIC_API(void)
JRI_SetFieldByName(JRIEnv* env, jref obj, JRIFieldThunk* field, jref value);

JRI_PUBLIC_API(void)
JRI_SetFieldByName_boolean(JRIEnv* env, jref obj, JRIFieldThunk* field, jbool value);

JRI_PUBLIC_API(void)
JRI_SetFieldByName_byte(JRIEnv* env, jref obj, JRIFieldThunk* field, jbyte value);

JRI_PUBLIC_API(void)
JRI_SetFieldByName_char(JRIEnv* env, jref obj, JRIFieldThunk* field, jchar value);

JRI_PUBLIC_API(void)
JRI_SetFieldByName_short(JRIEnv* env, jref obj, JRIFieldThunk* field, jshort value);

JRI_PUBLIC_API(void)
JRI_SetFieldByName_int(JRIEnv* env, jref obj, JRIFieldThunk* field, jint value);

JRI_PUBLIC_API(void)
JRI_SetFieldByName_long(JRIEnv* env, jref obj, JRIFieldThunk* field, jlong value);

JRI_PUBLIC_API(void)
JRI_SetFieldByName_float(JRIEnv* env, jref obj, JRIFieldThunk* field, jfloat value);

JRI_PUBLIC_API(void)
JRI_SetFieldByName_double(JRIEnv* env, jref obj, JRIFieldThunk* field, jdouble value);

/******************************************************************************/

JRI_PUBLIC_API(jref)
JRI_GetStaticFieldByName(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jbool)
JRI_GetStaticFieldByName_boolean(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jbyte)
JRI_GetStaticFieldByName_byte(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jchar)
JRI_GetStaticFieldByName_char(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jshort)
JRI_GetStaticFieldByName_short(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jint)
JRI_GetStaticFieldByName_int(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jlong)
JRI_GetStaticFieldByName_long(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jfloat)
JRI_GetStaticFieldByName_float(JRIEnv* env, jref obj, JRIFieldThunk* field);

JRI_PUBLIC_API(jdouble)
JRI_GetStaticFieldByName_double(JRIEnv* env, jref obj, JRIFieldThunk* field);

/******************************************************************************/

JRI_PUBLIC_API(void)
JRI_SetStaticFieldByName(JRIEnv* env, jref obj, JRIFieldThunk* field, jref value);

JRI_PUBLIC_API(void)
JRI_SetStaticFieldByName_boolean(JRIEnv* env, jref obj, JRIFieldThunk* field, jbool value);

JRI_PUBLIC_API(void)
JRI_SetStaticFieldByName_byte(JRIEnv* env, jref obj, JRIFieldThunk* field, jbyte value);

JRI_PUBLIC_API(void)
JRI_SetStaticFieldByName_char(JRIEnv* env, jref obj, JRIFieldThunk* field, jchar value);

JRI_PUBLIC_API(void)
JRI_SetStaticFieldByName_short(JRIEnv* env, jref obj, JRIFieldThunk* field, jshort value);

JRI_PUBLIC_API(void)
JRI_SetStaticFieldByName_int(JRIEnv* env, jref obj, JRIFieldThunk* field, jint value);

JRI_PUBLIC_API(void)
JRI_SetStaticFieldByName_long(JRIEnv* env, jref obj, JRIFieldThunk* field, jlong value);

JRI_PUBLIC_API(void)
JRI_SetStaticFieldByName_float(JRIEnv* env, jref obj, JRIFieldThunk* field, jfloat value);

JRI_PUBLIC_API(void)
JRI_SetStaticFieldByName_double(JRIEnv* env, jref obj, JRIFieldThunk* field, jdouble value);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* JRI_H */
/******************************************************************************/
