#pragma once

#include <cstddef>
#include <string>

enum class TokenType
{
  // Literals and names
  Number,      // 42, 3.14
  String,      // 'hello'
  Boolean,     // true, false
  Identifier,  // A, ready, msg

  // Arithmetic / concat
  Plus,    // +
  Minus,   // -
  Star,    // *
  Slash,   // /
  DotDot,  // ..  string concat, e.g. 'a' .. 'b'

  // Comparison
  EqualEqual,    // ==
  BangEqual,     // !=
  Less,          // <
  Greater,       // >
  LessEqual,     // <=
  GreaterEqual,  // >=

  // Logic
  AmpAmp,    // &&
  PipePipe,  // ||
  Bang,      // !

  // Assignment
  ColonEqual,  // :=  create or overwrite
  Equal,       // =   assign existing only
  PlusEqual,   // +=
  MinusEqual,  // -=
  StarEqual,   // *=
  SlashEqual,  // /=

  // Ternary
  Question,  // ?
  Colon,     // :   (ternary else, not :=)

  // Punctuation
  LeftParen,   // (
  RightParen,  // )
  Semicolon,   // ;

  EndOfInput,  // end of the source string
  Error,       // illegal char or unterminated string
};

struct Token
{
  TokenType type = TokenType::EndOfInput;
  std::string text;
  std::size_t position = 0;
};

inline const char* toStr(TokenType type)
{
  switch (type)
  {
    case TokenType::Number:
      return "Number";
    case TokenType::String:
      return "String";
    case TokenType::Boolean:
      return "Boolean";
    case TokenType::Identifier:
      return "Identifier";
    case TokenType::Plus:
      return "Plus";
    case TokenType::Minus:
      return "Minus";
    case TokenType::Star:
      return "Star";
    case TokenType::Slash:
      return "Slash";
    case TokenType::DotDot:
      return "DotDot";
    case TokenType::EqualEqual:
      return "EqualEqual";
    case TokenType::BangEqual:
      return "BangEqual";
    case TokenType::Less:
      return "Less";
    case TokenType::Greater:
      return "Greater";
    case TokenType::LessEqual:
      return "LessEqual";
    case TokenType::GreaterEqual:
      return "GreaterEqual";
    case TokenType::AmpAmp:
      return "AmpAmp";
    case TokenType::PipePipe:
      return "PipePipe";
    case TokenType::Bang:
      return "Bang";
    case TokenType::ColonEqual:
      return "ColonEqual";
    case TokenType::Equal:
      return "Equal";
    case TokenType::PlusEqual:
      return "PlusEqual";
    case TokenType::MinusEqual:
      return "MinusEqual";
    case TokenType::StarEqual:
      return "StarEqual";
    case TokenType::SlashEqual:
      return "SlashEqual";
    case TokenType::Question:
      return "Question";
    case TokenType::Colon:
      return "Colon";
    case TokenType::LeftParen:
      return "LeftParen";
    case TokenType::RightParen:
      return "RightParen";
    case TokenType::Semicolon:
      return "Semicolon";
    case TokenType::EndOfInput:
      return "EndOfInput";
    case TokenType::Error:
      return "Error";
  }
  return "Unknown";
}
