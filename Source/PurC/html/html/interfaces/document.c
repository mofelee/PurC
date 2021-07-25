/*
 * Copyright (C) 2018-2020 Alexander Borisov
 *
 * Author: Alexander Borisov <borisov@lexbor.com>
 */

#include "html/core/str.h"

#include "html/html/interfaces/document.h"
#include "html/html/interfaces/title_element.h"
#include "html/html/node.h"
#include "html/html/parser.h"

#include "html/tag/tag.h"

#include "html/dom/interfaces/text.h"
#include "html/dom/interfaces/element.h"

#define PCHTML_HTML_TAG_RES_DATA
#define PCHTML_HTML_TAG_RES_SHS_DATA
#include "html/html/tag_res.h"


unsigned int
pchtml_html_parse_chunk_prepare(pchtml_html_parser_t *parser,
                             pchtml_html_document_t *document);

static inline unsigned int
pchtml_html_document_parser_prepare(pchtml_html_document_t *document);

static pchtml_action_t
pchtml_html_document_title_walker(pchtml_dom_node_t *node, void *ctx);


pchtml_html_document_t *
pchtml_html_document_interface_create(pchtml_html_document_t *document)
{
    unsigned int status;
    pchtml_dom_document_t *doc;
    pchtml_dom_interface_create_f icreator;

    if (document != NULL) {
        doc = pchtml_mraw_calloc(pchtml_html_document_mraw(document),
                                 sizeof(pchtml_html_document_t));
    }
    else {
        doc = pchtml_calloc(1, sizeof(pchtml_html_document_t));
    }

    if (doc == NULL) {
        return NULL;
    }

    icreator = (pchtml_dom_interface_create_f) pchtml_html_interface_create;

    status = pchtml_dom_document_init(doc, pchtml_dom_interface_document(document),
                                   icreator, pchtml_html_interface_destroy,
                                   PCHTML_DOM_DOCUMENT_DTYPE_HTML, PCHTML_NS_HTML);
    if (status != PCHTML_STATUS_OK) {
        (void) pchtml_dom_document_destroy(doc);
        return NULL;
    }

    return pchtml_html_interface_document(doc);
}

pchtml_html_document_t *
pchtml_html_document_interface_destroy(pchtml_html_document_t *document)
{
    pchtml_dom_document_t *doc;

    if (document == NULL) {
        return NULL;
    }

    doc = pchtml_dom_interface_document(document);

    if (doc->node.owner_document == doc) {
        (void) pchtml_html_parser_unref(doc->parser);
    }

    (void) pchtml_dom_document_destroy(doc);

    return NULL;
}

pchtml_html_document_t *
pchtml_html_document_create(void)
{
    return pchtml_html_document_interface_create(NULL);
}

void
pchtml_html_document_clean(pchtml_html_document_t *document)
{
    document->body = NULL;
    document->head = NULL;
    document->iframe_srcdoc = NULL;
    document->ready_state = PCHTML_HTML_DOCUMENT_READY_STATE_UNDEF;

    pchtml_dom_document_clean(pchtml_dom_interface_document(document));
}

pchtml_html_document_t *
pchtml_html_document_destroy(pchtml_html_document_t *document)
{
    return pchtml_html_document_interface_destroy(document);
}

unsigned int
pchtml_html_document_parse(pchtml_html_document_t *document,
                        const unsigned char *html, size_t size)
{
    unsigned int status;
    pchtml_dom_document_t *doc;
    pchtml_html_document_opt_t opt;

    if (document->ready_state != PCHTML_HTML_DOCUMENT_READY_STATE_UNDEF
        && document->ready_state != PCHTML_HTML_DOCUMENT_READY_STATE_LOADING)
    {
        pchtml_html_document_clean(document);
    }

    opt = document->opt;
    doc = pchtml_dom_interface_document(document);

    status = pchtml_html_document_parser_prepare(document);
    if (status != PCHTML_STATUS_OK) {
        goto failed;
    }

    status = pchtml_html_parse_chunk_prepare(doc->parser, document);
    if (status != PCHTML_STATUS_OK) {
        goto failed;
    }

    status = pchtml_html_parse_chunk_process(doc->parser, html, size);
    if (status != PCHTML_STATUS_OK) {
        goto failed;
    }

    document->opt = opt;

    return pchtml_html_parse_chunk_end(doc->parser);

failed:

    document->opt = opt;

    return status;
}

unsigned int
pchtml_html_document_parse_chunk_begin(pchtml_html_document_t *document)
{
    if (document->ready_state != PCHTML_HTML_DOCUMENT_READY_STATE_UNDEF
        && document->ready_state != PCHTML_HTML_DOCUMENT_READY_STATE_LOADING)
    {
        pchtml_html_document_clean(document);
    }

    unsigned int status = pchtml_html_document_parser_prepare(document);
    if (status != PCHTML_STATUS_OK) {
        return status;
    }

    return pchtml_html_parse_chunk_prepare(document->dom_document.parser,
                                        document);
}

unsigned int
pchtml_html_document_parse_chunk(pchtml_html_document_t *document,
                              const unsigned char *html, size_t size)
{
    return pchtml_html_parse_chunk_process(document->dom_document.parser,
                                        html, size);
}

unsigned int
pchtml_html_document_parse_chunk_end(pchtml_html_document_t *document)
{
    return pchtml_html_parse_chunk_end(document->dom_document.parser);
}

pchtml_dom_node_t *
pchtml_html_document_parse_fragment(pchtml_html_document_t *document,
                                 pchtml_dom_element_t *element,
                                 const unsigned char *html, size_t size)
{
    unsigned int status;
    pchtml_html_parser_t *parser;
    pchtml_html_document_opt_t opt = document->opt;

    status = pchtml_html_document_parser_prepare(document);
    if (status != PCHTML_STATUS_OK) {
        goto failed;
    }

    parser = document->dom_document.parser;

    status = pchtml_html_parse_fragment_chunk_begin(parser, document,
                                                 element->node.local_name,
                                                 element->node.ns);
    if (status != PCHTML_STATUS_OK) {
        goto failed;
    }

    status = pchtml_html_parse_fragment_chunk_process(parser, html, size);
    if (status != PCHTML_STATUS_OK) {
        goto failed;
    }

    document->opt = opt;

    return pchtml_html_parse_fragment_chunk_end(parser);

failed:

    document->opt = opt;

    return NULL;
}

unsigned int
pchtml_html_document_parse_fragment_chunk_begin(pchtml_html_document_t *document,
                                             pchtml_dom_element_t *element)
{
    unsigned int status;
    pchtml_html_parser_t *parser = document->dom_document.parser;

    status = pchtml_html_document_parser_prepare(document);
    if (status != PCHTML_STATUS_OK) {
        return status;
    }

    return pchtml_html_parse_fragment_chunk_begin(parser, document,
                                               element->node.local_name,
                                               element->node.ns);
}

unsigned int
pchtml_html_document_parse_fragment_chunk(pchtml_html_document_t *document,
                                       const unsigned char *html, size_t size)
{
    return pchtml_html_parse_fragment_chunk_process(document->dom_document.parser,
                                                 html, size);
}

pchtml_dom_node_t *
pchtml_html_document_parse_fragment_chunk_end(pchtml_html_document_t *document)
{
    return pchtml_html_parse_fragment_chunk_end(document->dom_document.parser);
}

static inline unsigned int
pchtml_html_document_parser_prepare(pchtml_html_document_t *document)
{
    unsigned int status;
    pchtml_dom_document_t *doc;

    doc = pchtml_dom_interface_document(document);

    if (doc->parser == NULL) {
        doc->parser = pchtml_html_parser_create();
        status = pchtml_html_parser_init(doc->parser);

        if (status != PCHTML_STATUS_OK) {
            pchtml_html_parser_destroy(doc->parser);
            return status;
        }
    }
    else if (pchtml_html_parser_state(doc->parser) != PCHTML_HTML_PARSER_STATE_BEGIN) {
        pchtml_html_parser_clean(doc->parser);
    }

    return PCHTML_STATUS_OK;
}

const unsigned char *
pchtml_html_document_title(pchtml_html_document_t *document, size_t *len)
{
    pchtml_html_title_element_t *title = NULL;

    pchtml_dom_node_simple_walk(pchtml_dom_interface_node(document),
                             pchtml_html_document_title_walker, &title);
    if (title == NULL) {
        return NULL;
    }

    return pchtml_html_title_element_strict_text(title, len);
}

unsigned int
pchtml_html_document_title_set(pchtml_html_document_t *document,
                            const unsigned char *title, size_t len)
{
    unsigned int status;

    /* TODO: If the document element is an SVG svg element */

    /* If the document element is in the HTML namespace */
    if (document->head == NULL) {
        return PCHTML_STATUS_OK;
    }

    pchtml_html_title_element_t *el_title = NULL;

    pchtml_dom_node_simple_walk(pchtml_dom_interface_node(document),
                             pchtml_html_document_title_walker, &el_title);
    if (el_title == NULL) {
        el_title = (void *) pchtml_html_document_create_element(document,
                                         (const unsigned char *) "title", 5, NULL);
        if (el_title == NULL) {
            return PCHTML_STATUS_ERROR_MEMORY_ALLOCATION;
        }

        pchtml_dom_node_insert_child(pchtml_dom_interface_node(document->head),
                                  pchtml_dom_interface_node(el_title));
    }

    status = pchtml_dom_node_text_content_set(pchtml_dom_interface_node(el_title),
                                           title, len);
    if (status != PCHTML_STATUS_OK) {
        pchtml_html_document_destroy_element(&el_title->element.element);

        return status;
    }

    return PCHTML_STATUS_OK;
}

const unsigned char *
pchtml_html_document_title_raw(pchtml_html_document_t *document, size_t *len)
{
    pchtml_html_title_element_t *title = NULL;

    pchtml_dom_node_simple_walk(pchtml_dom_interface_node(document),
                             pchtml_html_document_title_walker, &title);
    if (title == NULL) {
        return NULL;
    }

    return pchtml_html_title_element_text(title, len);
}

static pchtml_action_t
pchtml_html_document_title_walker(pchtml_dom_node_t *node, void *ctx)
{
    if (node->local_name == PCHTML_TAG_TITLE) {
        *((void **) ctx) = node;

        return PCHTML_ACTION_STOP;
    }

    return PCHTML_ACTION_OK;
}

/*
 * No inline functions for ABI.
 */
pchtml_html_head_element_t *
pchtml_html_document_head_element_noi(pchtml_html_document_t *document)
{
    return pchtml_html_document_head_element(document);
}

pchtml_html_body_element_t *
pchtml_html_document_body_element_noi(pchtml_html_document_t *document)
{
    return pchtml_html_document_body_element(document);
}

pchtml_dom_document_t *
pchtml_html_document_original_ref_noi(pchtml_html_document_t *document)
{
    return pchtml_html_document_original_ref(document);
}

bool
pchtml_html_document_is_original_noi(pchtml_html_document_t *document)
{
    return pchtml_html_document_is_original(document);
}

pchtml_mraw_t *
pchtml_html_document_mraw_noi(pchtml_html_document_t *document)
{
    return pchtml_html_document_mraw(document);
}

pchtml_mraw_t *
pchtml_html_document_mraw_text_noi(pchtml_html_document_t *document)
{
    return pchtml_html_document_mraw_text(document);
}

void
pchtml_html_document_opt_set_noi(pchtml_html_document_t *document,
                              pchtml_html_document_opt_t opt)
{
    pchtml_html_document_opt_set(document, opt);
}

pchtml_html_document_opt_t
pchtml_html_document_opt_noi(pchtml_html_document_t *document)
{
    return pchtml_html_document_opt(document);
}

void *
pchtml_html_document_create_struct_noi(pchtml_html_document_t *document,
                                    size_t struct_size)
{
    return pchtml_html_document_create_struct(document, struct_size);
}

void *
pchtml_html_document_destroy_struct_noi(pchtml_html_document_t *document, void *data)
{
    return pchtml_html_document_destroy_struct(document, data);
}

pchtml_html_element_t *
pchtml_html_document_create_element_noi(pchtml_html_document_t *document,
                                     const unsigned char *local_name,
                                     size_t lname_len, void *reserved_for_opt)
{
    return pchtml_html_document_create_element(document, local_name, lname_len,
                                            reserved_for_opt);
}

pchtml_dom_element_t *
pchtml_html_document_destroy_element_noi(pchtml_dom_element_t *element)
{
    return pchtml_html_document_destroy_element(element);
}
