//===--------------------------------------------------------------------------------------------===
// lexer.cpp - House::AssetParser lexing methods.
// This source is part of the House Engine
//
// Created on 2019-01-30 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <algorithm>
#include <cctype>
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <house/parser.hpp>

namespace House {
    
    static std::string toLower(const std::string& str) {
        std::string lower = str;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return lower;
    }

    // TODO: in the future, we probably want to load allowable articles from a dev's file?
    // TODO: replace these with sets
    static const std::vector<std::string> articles = {
        "a", "an", "the", "some", "they"
    };

    static const std::vector<std::string> descriptives = {
        "this", "that", "it", "they", "them" // TODO: Really, we should have pronounds too
    };

    static const std::vector<std::string> prepositions = {
        "to", "from", "on", "above", "at"
    };

    std::size_t AssetParser::remaining() const {
        return file_.length() - ptr_;
    }

    codepoint_t AssetParser::current() const {
        return utf8_getCodepoint(file_.data() + ptr_, remaining());
    }

    codepoint_t AssetParser::nextChar() {
        if(ptr_ >= file_.size()) return current_ = '\0';
    
        current_ = current();
        auto size = utf8_codepointSize(current_);
        if(size > 0) {
            ptr_ += size;
        }
        return current_;
    }

    bool AssetParser::isIdentifier(codepoint_t c) {
        return utf8_isIdentifier(c)
            || c == '-'
            || c == '\'';
    }

    void AssetParser::updateTokenStart() {
        start_ = ptr_;
    }

    const Token& AssetParser::lexKeyword() {
        while(isIdentifier(current())) {
            nextChar();
        }
    
        const auto length = ptr_ - start_;
        const auto str = file_.substr(start_, length);
    
        if (str == "#objects") {
            return makeToken(Token::Objects);
        }
        if (str == "#links") {
            return makeToken(Token::Links);
        }
        std::cerr << "Invalid section keyword '" << str << "' in '" << key_ << "'\n";
        abort();
    }

    const Token& AssetParser::lexString() {
    
        while(current() != '"') {
            codepoint_t c = nextChar();
        
            //if(c == '"') break;
            if(c == '\0') {
                std::cerr << "quoted string not finished in '" << key_ << "'\n";
                abort();
            }
        }
        nextChar();
    
        const auto length = (ptr_ - start_)-2;
        return makeToken(Token::QuotedString, file_.substr(start_+1, length));
    }

    const Token& AssetParser::lexWord() {
        while(isIdentifier(current())) {
            nextChar();
        }
    
        const auto length = ptr_ - start_;
        const auto str = file_.substr(start_, length);
        const auto low = toLower(str);
    
        // Check if it's an article!
        // TODO: we probably need some sort of case-insensitive compare here :/
        if(std::find(articles.begin(), articles.end(), low) != articles.end())
            return makeToken(Token::Article, str);
        if(std::find(descriptives.begin(), descriptives.end(), low) != descriptives.end())
            return makeToken(Token::Descriptive, str);
        if(low == "is" || low == "are")
            return makeToken(Token::Be, str);
        if(low == "can")
            return makeToken(Token::Can, str);
        if(low == "you")
            return makeToken(Token::You, str);
        return makeToken(Token::Word, str);
    }

    const Token& AssetParser::lexNumber() {
        while(current() >= '0' && current() < '9') {
            nextChar();
        }
        const auto length = (ptr_ - start_);
        return makeToken(Token::Article, file_.substr(start_, length));
    }

    const Token& AssetParser::makeToken(Token::Kind kind, const std::string& str) {
        currentToken_.kind = kind;
        currentToken_.text = str;
        //std::cout << "[ " << currentToken_.kind << " ] " << currentToken_.text << "\n";
        return currentToken_;
    }

    const Token& AssetParser::nextToken() {
        if(ptr_ >= file_.size()) {
            return makeToken(Token::End);
        }
    
        while(current() != '\0') {
            updateTokenStart();
            codepoint_t c = nextChar();
        
            switch(c) {
                //whitespace
                case 0x0020:
                case 0x000d:
                case 0x0009:
                case 0x000b:
                case 0x000c:
                case 0x000a:
                case 0xfeff:
                    break;

                // // line feed
                //     return makeToken(Token::Newline);
                    
                case '.':
                    return makeToken(Token::Period);
            
                case ',':
                    return makeToken(Token::Comma);
                
                case '&':
                    return makeToken(Token::Amp);
                
                case '-':
                    return makeToken(Token::Dash);
            
                case '#':
                    return lexKeyword();
                
                case '"':
                    return lexString();
                    
                case ':':
                    return makeToken(Token::Colon);
                
                default:
                    if (utf8_isIdentifierHead(c)) {
                        return lexWord();
                    }
                    std::cerr << "Invalid character in room file '" << key_ << "': "<< (char)c <<"\n";
                    abort();
            }
        }
        return makeToken(Token::End);
    }
}