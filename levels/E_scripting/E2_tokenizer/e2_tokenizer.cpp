#include "mini_bt/tokenizer.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace
{

void printTokens(const std::string& source)
{
  const auto tokens = tokenize(source);
  std::cout << "source: " << source << "\n";
  std::cout << "tokens:\n";
  for (const auto& token : tokens)
  {
    std::cout << "  " << toStr(token.type) << "  text='" << token.text
              << "'  pos=" << token.position << "\n";
  }
  std::cout << "\n";
}

bool checkTypes(const std::string& source, const std::vector<TokenType>& expected)
{
  const auto tokens = tokenize(source);
  if (tokens.size() != expected.size())
  {
    std::cout << "FAIL [" << source << "]: size " << tokens.size() << " != "
              << expected.size() << "\n";
    return false;
  }
  for (std::size_t i = 0; i < expected.size(); ++i)
  {
    if (tokens[i].type != expected[i])
    {
      std::cout << "FAIL [" << source << "] at " << i << ": got "
                << toStr(tokens[i].type) << " expected " << toStr(expected[i]) << "\n";
      return false;
    }
  }
  return true;
}

}  // namespace

int main()
{
  const std::string demo = "A:=42; B>3 && true";
  printTokens(demo);

  bool ok = true;
  ok &= checkTypes(demo, {TokenType::Identifier, TokenType::ColonEqual, TokenType::Number,
                          TokenType::Semicolon, TokenType::Identifier, TokenType::Greater,
                          TokenType::Number, TokenType::AmpAmp, TokenType::Boolean,
                          TokenType::EndOfInput});
  ok &= checkTypes("x==1", {TokenType::Identifier, TokenType::EqualEqual, TokenType::Number,
                            TokenType::EndOfInput});
  ok &= checkTypes("a?b:c", {TokenType::Identifier, TokenType::Question, TokenType::Identifier,
                             TokenType::Colon, TokenType::Identifier, TokenType::EndOfInput});
  ok &= checkTypes("'hi' .. '!'",
                   {TokenType::String, TokenType::DotDot, TokenType::String,
                    TokenType::EndOfInput});
  ok &= checkTypes("3.14", {TokenType::Number, TokenType::EndOfInput});
  ok &= checkTypes("65..66", {TokenType::Number, TokenType::DotDot, TokenType::Number,
                              TokenType::EndOfInput});
  ok &= checkTypes("'oops", {TokenType::Error, TokenType::EndOfInput});
  ok &= checkTypes("@", {TokenType::Error, TokenType::EndOfInput});

  if (!ok)
  {
    std::cout << "E2 tokenizer checks failed\n";
    return 1;
  }
  std::cout << "E2 tokenizer checks passed\n";
  return 0;
}
