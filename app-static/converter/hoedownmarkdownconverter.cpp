/*
 * Copyright 2013 Christian Loose <christian.loose@hamburg.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "hoedownmarkdownconverter.h"

extern "C" {
#ifdef Q_OS_WIN
#include <Windows.h>
#include <src/document.h>
#include <src/html.h>
#else
#include <hoedown/document.h>
#include <hoedown/html.h>
#endif
}

#include "markdowndocument.h"

class HoedownMarkdownDocument : public MarkdownDocument
{
public:
    explicit HoedownMarkdownDocument(hoedown_buffer *document, unsigned long options)
        : document_(document)
        , options_(options)
    {}
    ~HoedownMarkdownDocument() { hoedown_buffer_free(document_); }

    hoedown_buffer *document() const { return document_; }
    unsigned long options() const { return options_; }

private:
    hoedown_buffer *document_;
    unsigned long options_;
};


HoedownMarkdownConverter::HoedownMarkdownConverter()
{
}

MarkdownDocument *HoedownMarkdownConverter::createDocument(const QString &text, ConverterOptions options)
{
    hoedown_buffer *doc = 0;

    if (text.length() > 0) {
        QString markdownText(text);

        // text has to always end with a line break,
        // otherwise characters are missing in HTML
        if (!markdownText.endsWith('\n')) {
            markdownText.append('\n');
        }

        QByteArray utf8Data = markdownText.toUtf8();
        doc = hoedown_buffer_new(utf8Data.length());
        hoedown_buffer_puts(doc, utf8Data.data());
    }

    return new HoedownMarkdownDocument(doc, translateConverterOptions(options));
}

QString HoedownMarkdownConverter::renderAsHtml(MarkdownDocument *document)
{
    QString result;

    if (document) {
        HoedownMarkdownDocument *doc = dynamic_cast<HoedownMarkdownDocument*>(document);

        if (doc->document()) {
            hoedown_buffer *in = doc->document();
            hoedown_buffer *html = hoedown_buffer_new(64);

            hoedown_renderer *renderer = hoedown_html_renderer_new((hoedown_html_flags)0, 16);
            hoedown_document *d = hoedown_document_new(renderer, (hoedown_extensions)(doc->options()), 16);
            hoedown_document_render(d, html, in->data, in->size);

            result = QString::fromUtf8(hoedown_buffer_cstr(html));

            hoedown_buffer_free(html);
            hoedown_document_free(d);
            hoedown_html_renderer_free(renderer);
        }
    }

    return result;
}

QString HoedownMarkdownConverter::renderAsTableOfContents(MarkdownDocument *document)
{
    QString toc;
    return toc;
}

MarkdownConverter::ConverterOptions HoedownMarkdownConverter::supportedOptions() const
{
    return MarkdownConverter::AutolinkOption |
           MarkdownConverter::NoStrikethroughOption |
           MarkdownConverter::NoAlphaListOption |
           MarkdownConverter::NoDefinitionListOption |
           MarkdownConverter::NoSmartypantsOption |
           MarkdownConverter::ExtraFootnoteOption |
           MarkdownConverter::NoSuperscriptOption;
}

unsigned long HoedownMarkdownConverter::translateConverterOptions(ConverterOptions options) const
{
    unsigned long converterOptions = HOEDOWN_EXT_FENCED_CODE | HOEDOWN_EXT_TABLES | HOEDOWN_EXT_AUTOLINK;

    // strikethrough
    if (options.testFlag(MarkdownConverter::NoStrikethroughOption)) {
        converterOptions |= HOEDOWN_EXT_STRIKETHROUGH;
    }

    // Footnotes
    if (options.testFlag(MarkdownConverter::ExtraFootnoteOption)) {
        converterOptions |= HOEDOWN_EXT_FOOTNOTES;
    }

    // Superscript
    if (options.testFlag(MarkdownConverter::NoSuperscriptOption)) {
        converterOptions |= HOEDOWN_EXT_SUPERSCRIPT;
    }

    return converterOptions;
}
