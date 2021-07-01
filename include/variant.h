/*
** Copyright (C) 2021 FMSoft <https://www.fmsoft.cn>
**
** This file is a part of Purring Cat 2, a HVML parser and interpreter.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
**
*/
// 头文件说明改

#ifndef PURC_VARIANT_H
#define PURC_VARIANT_H

#pragma once

struct _PURC_VARIANT;

typedef struct _PURC_VARIANT PURC_VARIANT;
typedef struct _PURC_VARIANT* purc_variant_t;

#define PURC_VARIANT_UNDEFINED          ((purc_variant_t)(-1))
#define PURC_VARIANT_NULL               ((purc_variant_t)(0))

// TODO
#define PURC_VARIANT_TRUE               ((purc_variant_t)(1))
#define PURC_VARIANT_FALSE              ((purc_variant_t)(2))


/**
 * Creates a variant value of undefined type.
 *
 * Returns: A purc_variant_t with undefined type.
 *
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_undefined (void);


/**
 * Creates a variant value of null type.
 *
 * Returns: A purc_variant_t with null type.
 *
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_null (void);


/**
 * Creates a variant value of boolean type.
 *
 * @param b: the initial value of created data
 *
 * Returns: A purc_variant_t with boolean type.
 *
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_boolean (bool b);


/**
 * Creates a variant value of number type.
 *
 * @param d: the initial value of created data
 *
 * Returns: A purc_variant_t with number type, or with undefined 
 *          type on failure.
 *
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_number (double d);


/**
 * Creates a variant value of long int type.
 *
 * @param u64: the initial value of unsigned long int type
 *
 * Returns: A purc_variant_t with long int type, or with undefined
 *          type on failure.
 *
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_unsigned_longint (uint64_t u64);


/**
 * Creates a variant value of long int type.
 *
 * @param u64: the initial value of signed long int type
 *
 * Returns: A purc_variant_t with long int type, or with undefined 
 *          type on failure.
 *
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_signed_longint (uint64_t u64);


/**
 * Creates a variant value of string type.
 *
 * @param str_utf8: the pointer of a string which is in UTF-8 encoding 
 *
 * Returns: A purc_variant_t with string type, or with undefined 
 *          type on failure.
 *
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_string (const char* str_utf8);


/**
 * Checks the encoding format of input parameter, and creates a variant 
 * value of string type.
 *
 * @param str_utf8: the pointer of a string which is in UTF-8 encoding
 *
 * Returns: A purc_variant_t with string type, or with undefined 
 *          type on failure.
 *
 * Note: If str_utf8 is not in UTF-8 encoding, return undefined type.
 *
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_string_with_check (const char* str_utf8);


/**
 * Gets the pointer of string which is encapsulated in string type.
 *
 * @param value: the data of string type
 *
 * Returns: The pointer of char string, or NULL if value with undefined type.
 *
 * Since: 0.0.1
 */
const char* purc_variant_get_string_const (purc_variant_t value);


/**
 * Get the number of characters in an string variant value.
 *
 * @param string: the variant value of string type
 *
 * Returns: The number of characters in an string variant value.
 *
 * Since: 0.0.1
 */
size_t purc_variant_string_length(const purc_variant_t string);


/**
 * Creates a variant value of byte sequence type.
 *
 * @param bytes: the pointer of a byte sequence
 * @param nr_bytes: the number of bytes in sequence
 *
 * Returns: A purc_variant_t with byte sequence type, or with undefined 
 *          type on failure.
 *
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_byte_sequence (const unsigned char* bytes, size_t nr_bytes);


/**
 * Gets the pointer of byte array which is encapsulated in byte sequence type.
 *
 * @param value: the data of byte sequence type
 * @param nr_bytes: the size of byte sequence
 *
 * Returns: the pointer of byte array on success, or NULL on failure.
 *
 * Since: 0.0.1
 */
const unsigned char* purc_variant_get_bytes_const (purc_variant_t value, size_t* nr_bytes);


/**
 * Get the number of bytes in an sequence variant value.
 *
 * @param sequence: the variant value of sequence type
 *
 * Returns: The number of bytes in an sequence variant value.
 *
 * Since: 0.0.1
 */
size_t purc_variant_sequence_length(const purc_variant_t sequence);



typedef purc_variant_t (*PCB_DYNAMIC_VARIANT) (purc_variant_t root, int nr_args, purc_variant_t arg0, ...);


/**
 * Creates dynamic value by setter and getter functions
 *
 * @param getter: the getter funciton pointer
 * @param setter: the setter function pointer
 *
 * Returns: A purc_variant_t with dynamic value, or undefined 
 *          type on failure.
 *
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_dynamic_value (CB_DYNAMIC_VARIANT getter, CB_DYNAMIC_VARIANT setter);


/**
 * Creates a variant value of array type.
 *
 * @param sz: the size of array
 * @param value0 ..... valuen: enumerates every elements in array 
 *
 * Returns: A purc_variant_t with array type, or undefined
 *          type on failure.
 *
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_array (size_t sz, purc_variant_t value0, ...);


/**
 * Appends a variant data to the tail of an array.
 *
 * @param array: the variant data of array type
 * @param value: the element to be appended
 *
 * Returns: Zero on success, or -1 on failure.
 *
 * Since: 0.0.1
 */
int purc_variant_array_append (purc_variant_t array, purc_variant_t value);


/**
 * Gets an element from an array by index.
 *
 * @param array: the variant data of array type
 * @param idx: the index of wanted element 
 *
 * Returns: A purc_variant_t on success, or undefined type  on failure.
 *
 * Since: 0.0.1
 */
 ???
purc_variant_t purc_variant_array_get (purc_variant_t array, int idx);


/**
 * Sets an element value in an array by index.
 *
 * @param array: the variant data of array type
 * @param idx: the index of replaced element 
 * @param value: the element to replace
 *
 * Returns: Zero on success, or -1 on failure.
 *
 * Note: If idx is greater than max index of array, return -1. 
 *       Whether free the replaced element, depends on its ref.
 *
 * Since: 0.0.1
 */
int purc_variant_array_set (purc_variant_t array, int idx, purc_variant_t value);


/**
 * Remove an element from an array by index.
 *
 * @param array: the variant data of array type
 * @param idx: the index of element to be removed
 *
 * Returns: Zero on success, or -1 on failure.
 *
 * Note: If idx is greater than max index of array, return -1.
 *       Whether free the removed element, depends on its ref.
 *
 * Since: 0.0.1
 */
int purc_variant_array_remove (purc_variant_t array, int idx);


/**
 * Inserts an element to an array, places it before an indicated element.
 *
 * @param array: the variant data of array type
 * @param idx: the index of element before which the new element will be placed
 *
 * @param value: the inserted element 
 *
 * Returns: Zero on success, or -1 on failure.
 *
 * Note: If idx is greater than max index of array, return -1.
 *
 * Since: 0.0.1
 */
int purc_variant_array_insert_before (purc_variant_t array, int idx, purc_variant_t value);


/**
 * Inserts an element to an array, places it after an indicated element.
 *
 * @param array: the variant data of array type
 * @param idx: the index of element after which the new element will be placed
 * @param value: the inserted element 
 *
 * Returns: Zero on success, or -1 on failure.
 *
 * Note: If idx is greater than sum of one plus max index of array, return -1.
 *
 * Since: 0.0.1
 */
int purc_variant_array_insert_after (purc_variant_t array, int idx, purc_variant_t value);


/**
 * Get the number of elements in an array variant value.
 *
 * @param array: the variant value of array type
 *
 * Returns: The number of elements in an array variant value.
 *
 * Since: 0.0.1
 */
size_t purc_variant_array_length(const purc_variant_t array);


/**
 * Creates a variant value of object type.
 *
 * @param nr_kv_pairs: the minimum of key-value pairs
 * @param key0 ..... keyn: the keys of key-value pairs 
 * @param value0 ..... valuen: the values of key-value pairs 
 *
 * Returns: A purc_variant_t with object type, or with undefined
 *          type on failure.
 *
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_object (size_t nr_kv_pairs, const char* key0, purc_variant_t value0, ...);


/**
 * Gets the value by key from an object.
 *
 * @param obj: the variant data of obj type
 * @param key: the key of key-value pair 
 *
 * Returns: A purc_variant_t on success, or with undefined
 *          type on failure.
 *
 * Since: 0.0.1
 */
???
purc_variant_t purc_variant_object_get (purc_variant_t obj, const char* key);


/**
 * Sets the value by key in an object.
 *
 * @param obj: the variant data of obj type
 * @param key: the key of key-value pair 
 * @param value: the value of key-value pair
 *
 * Returns: True on success, otherwise False.
 *
 * Since: 0.0.1
 */
bool purc_variant_object_set (purc_variant_t obj, const char* key, purc_variant_t value);


/**
 * Remove a key-value pair from an object by key.
 *
 * @param obj: the variant data of obj type
 * @param key: the key of key-value pair 
 *
 * Returns: True on success, otherwise False.
 *
 * Since: 0.0.1
 */
bool purc_variant_object_remove (purc_variant_t obj, const char* key);


/**
 * Get the number of key-value pairs in an object variant value.
 *
 * @param obj: the variant value of object type
 *
 * Returns: The number of key-value pairs in an object variant value.
 *
 * Since: 0.0.1
 */
size_t purc_variant_object_length(const purc_variant_t obj);


/**
 * Creates a variant data of set type.
 *
 * @param sz: the number of elements in a set
 * @param unique_key0 ..... unique_keyn: the keys of unique value 
 * @param value0 ..... valuen: the values related to the key. 
 *
 * Returns: A purc_variant_t on success, with undefined type on failure.
 *
 * Note: The key is legal, only when the value is object type.
 * 
 * Since: 0.0.1
 */
purc_variant_t purc_variant_make_set (size_t sz, const char* unique_key, purc_variant_t value0, ...);


/**
 * Adds a unique key-value pair to a set.
 *
 * @param set: the set to be added
 * @param value: the value to be added 
 *
 * Returns: True on success, False on failure
 *
 * Since: 0.0.1
 */
bool purc_variant_set_add (purc_variant_t set, purc_variant_t value);


/**
 * Remove a unique key-value pair from a set.
 *
 * @param set: the set to be operated 
 * @param value: the value to be removed
 *
 * Returns: True on success, False on failure
 *
 * Since: 0.0.1
 */
bool purc_variant_set_remove (purc_variant_t set, purc_variant_t value);


/**
 * Gets the value by key from a set.
 *
 * @param set: the variant data of obj type
 * @param match_key: the unique key related to the value 
 *
 * Returns: A purc_variant_t on success, with undefined type on failure.
 *
 * Since: 0.0.1
 */
 ???
purc_variant_t purc_variant_get_value_in_set (const purc_variant_t set, const char * match_key);


/**
 * Get the number of elements in a set variant value.
 *
 * @param set: the variant value of set type
 *
 * Returns: The number of elements in a set variant value.
 *
 * Since: 0.0.1
 */
size_t purc_variant_set_length(const purc_variant_t set);


/**
 * Adds ref for a variant data
 *
 * @param value: variant data to be operated
 *
 * Returns: A purc_variant_t on success, NULL on failure.
 *
 * Since: 0.0.1
 */
int purc_variant_ref (purc_variant_t value);


/**
 * substract ref for a variant data. When ref is zero, releases the resource occupied by the data
 *
 * @param value: variant data to be operated
 *
 * Note: When the ref is zero, the system will release all resource ocupied by value.
 *
 * Since: 0.0.1
 */
int purc_variant_unref (purc_variant_t value);


/**
 * Creates a variant data from a string which contents Json data
 *
 * @param json: the pointer of string which contents json data
 *
 * @param sz: the size of string 
 *
 * Returns: A purc_variant_t on success, with undefined type on failure.
 *
 * Since: 0.0.1
 */
 ????
purc_variant_t purc_variant_make_from_json_string (const char* json, size_t sz);


/**
 * Creates a variant data from Json file 
 *
 * @param file: the Json file name
 *
 * Returns: A purc_variant_t on success, with undefined type on failure.
 *
 * Since: 0.0.1
 */
 ???
purc_variant_t purc_variant_load_from_json_file (const char* file);


/**
 * Creates a variant data from stream 
 *
 * @param stream: the stream of purc_rwstream_t type
 *
 * Returns: A purc_variant_t on success, with undefined type on failure.
 *
 * Since: 0.0.1
 */
 ???
purc_variant_t purc_variant_load_from_json_stream (purc_rwstream_t stream);


/**
 * Compares two variant data 
 *
 * @param v1: one of compared variant data
 * @param v2: the other variant data to be compared
 *
 * Returns: return zero for identical, otherwise -1.
.*
 * Since: 0.0.1
 */
int purc_variant_cmp (purc_variant_t v1, purc_variant v2);


/**
 * Serialize a variant data 
 *
 * @param value: the variant data to be serialized
 *
 * @param steam: the stream to which the serialized data write
 * @param opts: the serialization options       // To be defined
 *
 * Returns: return the size of serialized data.
.*
 * Since: 0.0.1
 */
size_t purc_variant_serialize (purc_variant_t value, purc_rwstream_t stream, unsigned int opts);


/**
 * Loads a variant data from an indicated library
 *
 * @param so_name: the library name
 *
 * @param var_name: the variant data name
 *
 * Returns: A purc_variant_t on success, with undefined type on failure.
.*
 * Since: 0.0.1
 */
 ???
purc_variant_t purc_variant_dynamic_value_load_from_so (const char* so_name, const char* var_name);



typedef enum variant_type
{
    variant_type_null,
    variant_type_undefined,
    variant_type_boolean,
    variant_type_number,
    variant_type_longint,
    variant_type_string,
    variant_type_sequence,
    variant_type_dynamic,
    variant_type_object,
    variant_type_array,
    variant_type_set,
} variant_type;


/**
 * Whether the vairant is indicated type.
 *
 * @param value: the variant value
 * @param type: wanted type 
 *
 * Returns: True on success, otherwise False.
 *
 * Since: 0.0.1
 */
bool purc_variant_is_type(const purc_variant_t value, enum variant_type type);


/**
 * Get the type of a vairant value.
 *
 * @param value: the variant value
 *
 * Returns: The type of input variant value
 *
 * Since: 0.0.1
 */
enum variant_type purc_variant_get_type(const purc_variant_t value);


#endif /* PURC_VARIANT_H */

