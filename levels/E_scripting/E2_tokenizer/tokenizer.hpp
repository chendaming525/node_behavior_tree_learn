#pragma once

#include "mini_bt/token.hpp"

#include <string>
#include <vector>

class Tokenizer
{
public:
  explicit Tokenizer(std::string source);
  std::vector<Token> scanTokens();

private:
  bool isAtEnd() const;
  char peek() const;
  char peekNext() const;
  char advance();
  bool match(char expected);
  bool isDigit(char c) const;
  bool isAlpha(char c) const;
  bool isAlphaNumeric(char c) const;
  void scanToken();
  void scanNumber();
  void scanIdentifier();
  void scanString();
  void addToken(TokenType type, std::size_t start);
  void addToken(TokenType type, std::size_t start, std::string text);

  std::string source_;
  std::size_t current_ = 0;
  std::vector<Token> tokens_;
};

// Same public entry as BehaviorTree.CPP: one call, one result vector.
std::vector<Token> tokenize(const std::string& source);
