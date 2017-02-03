/*!
 * Title ---- km/TextNode.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_TEXT_NODE_HPP__
#define __KM_TEXT_NODE_HPP__

#include <km.hpp>
#include <km/Exception.hpp>
#include <km/Buffer.hpp>

#include <algorithm>

namespace km { // Begin main namespace

class Encrypter;

/*!
 * The text node.
 */
class TextNode
{
    friend class Encrypter;

public:

    /*!
     * Creates a text node.
     *
     * @param[in] content The content.
     *
     * @return The text node.
     */
    static TextNode create(const Buffer &content);

    /*!
     * Creates a text node.
     *
     * @param[in] content The content.
     * @param[in] nonce   The nonce.
     *
     * @return The text node.
     */
    static TextNode create(const Buffer &content, const Buffer &nonce);

    /*!
     * Creates a text node from a buffer.
     *
     * @param[in] data The data buffer.
     *
     * @return The text node.
     */
    static TextNode fromBuffer(const Buffer &data); // TODO: change this name

    /*!
     * Converts a text node into a buffer.
     *
     * @param[in] textNode The text node.
     *
     * @return The buffer.
     */
    static Buffer toBuffer(const TextNode &node);

    /*!
     * Destructor method.
     */
	virtual ~TextNode();

    /*!
     * Sets the value.
     *
     * @param[in] value The value.
     */
    TextNode &setContent(const Buffer &value);

    /*!
     * Returns the value.
     */
    const Buffer &getContent() const;

    /*!
     * Sets the nonce.
     */
    TextNode &setNonce(const Buffer &nonce);

    /*!
     * Returns the nonce.
     */
    const Buffer &getNonce() const;

    /*!
     * Converts the text node into a buffer.
     */
    Buffer toBuffer() const;

protected:

    /*!
     * Constructor method.
     */
    TextNode();


    /*!
     * The content.
     */
    Buffer mContent;

    /*!
     * The nonce.
     */
    Buffer mNonce;
};

} // End of main namespace

#endif /* __KM_TEXT_NODE_HPP__ */

// Include inline methods
#include <km/TextNode-inl.hpp>