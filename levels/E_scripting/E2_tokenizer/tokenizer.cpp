#include "mini_bt/tokenizer.hpp"

Tokenizer::Tokenizer(std::string source) : source_(std::move(source)) {}

std::vector<Token> Tokenizer::scanTokens()
{
  while (!isAtEnd())
  {
    scanToken();
  }
  tokens_.push_back(Token{TokenType::EndOfInput, "", source_.size()});
  return tokens_;
}

bool Tokenizer::isAtEnd() const
{
  return current_ >= source_.size();
}

char Tokenizer::peek() const
{
  if (isAtEnd())
  {
    return '\0';
  }
  return source_[current_];
}

char Tokenizer::peekNext() const
{
  if (current_ + 1 >= source_.size())
  {
    return '\0';
  }
  return source_[current_ + 1];
}

char Tokenizer::advance()
{
  if (isAtEnd())
  {
    return '\0';
  }
  return source_[current_++];
}

bool Tokenizer::match(char expected)
{
  if (peek() != expected)
  {
    return false;
  }
  advance();
  return true;
}

bool Tokenizer::isDigit(char c) const
{
  return c >= '0' && c <= '9';
}

bool Tokenizer::isAlpha(char c) const
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Tokenizer::isAlphaNumeric(char c) const
{
  return isAlpha(c) || isDigit(c);
}

void Tokenizer::addToken(TokenType type, std::size_t start)
{
  addToken(type, start, source_.substr(start, current_ - start));
}

void Tokenizer::addToken(TokenType type, std::size_t start, std::string text)
{
  tokens_.push_back(Token{type, std::move(text), start});
}

void Tokenizer::scanToken()
{
  const char c = peek();

  if (isDigit(c))
  {
    scanNumber();
    return;
  }
  if (isAlpha(c))
  {
    scanIdentifier();
    return;
  }
  if (c == '\'')
  {
    scanString();
    return;
  }

  const std::size_t start = current_;
  advance();

  switch (c)
  {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      return;
    case '(':
      addToken(TokenType::LeftParen, start);
      return;
    case ')':
      addToken(TokenType::RightParen, start);
      return;
    case ';':
      addToken(TokenType::Semicolon, start);
      return;
    case '?':
      addToken(TokenType::Question, start);
      return;
    case '+':
      addToken(match('=') ? TokenType::PlusEqual : TokenType::Plus, start);
      return;
    case '-':
      addToken(match('=') ? TokenType::MinusEqual : TokenType::Minus, start);
      return;
    case '*':
      addToken(match('=') ? TokenType::StarEqual : TokenType::Star, start);
      return;
    case '/':
      addToken(match('=') ? TokenType::SlashEqual : TokenType::Slash, start);
      return;
    case '=':
      addToken(match('=') ? TokenType::EqualEqual : TokenType::Equal, start);
      return;
    case '!':
      addToken(match('=') ? TokenType::BangEqual : TokenType::Bang, start);
      return;
    case '<':
      addToken(match('=') ? TokenType::LessEqual : TokenType::Less, start);
      return;
    case '>':
      addToken(match('=') ? TokenType::GreaterEqual : TokenType::Greater, start);
      return;
    case ':':
      addToken(match('=') ? TokenType::ColonEqual : TokenType::Colon, start);
      return;
    case '.':
      addToken(match('.') ? TokenType::DotDot : TokenType::Error, start);
      return;
    case '&':
      addToken(match('&') ? TokenType::AmpAmp : TokenType::Error, start);
      return;
    case '|':
      addToken(match('|') ? TokenType::PipePipe : TokenType::Error, start);
      return;
    default:
      addToken(TokenType::Error, start);
      return;
  }
}

void Tokenizer::scanNumber()
{
  const std::size_t start = current_;
  while (isDigit(peek()))
  {
    advance();
  }
  // 3.14 is a number; 65..66 is Integer 65 then DotDot.
  if (peek() == '.' && isDigit(peekNext()))
  {
    advance();
    while (isDigit(peek()))
    {
      advance();
    }
  }
  addToken(TokenType::Number, start);
}

void Tokenizer::scanIdentifier()
{
  const std::size_t start = current_;
  while (isAlphaNumeric(peek()))
  {
    advance();
  }
  const std::string text = source_.substr(start, current_ - start);
  if (text == "true" || text == "false")
  {
    addToken(TokenType::Boolean, start);
  }
  else
  {
    addToken(TokenType::Identifier, start);
  }
}

void Tokenizer::scanString()
{
  const std::size_t start = current_;
  advance();  // opening '
  const std::size_t content_start = current_;
  while (!isAtEnd() && peek() != '\'')
  {
    advance();
  }
  if (isAtEnd())
  {
    addToken(TokenType::Error, start);
    return;
  }
  const std::string content = source_.substr(content_start, current_ - content_start);
  advance();  // closing '
  addToken(TokenType::String, start, content);
}

std::vector<Token> tokenize(const std::string& source)
{
  Tokenizer tokenizer(source);
  return tokenizer.scanTokens();
}
