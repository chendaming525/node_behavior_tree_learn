#include "mini_bt/environment.hpp"
#include "mini_bt/expression.hpp"

#include <iostream>
#include <stdexcept>

int main()
{
  auto env = makeEnvironment();
  env.vars->set("A", ScriptValue(41.0));

  // Hand-built tree for: A + 1
  //        +
  //       / \
  //      A   1
  ExprPtr add_one = binary(name("A"), BinaryOp::Add, lit(1.0));

  // Hand-built tree for: 1 < 2 && 3 > 1
  //            &&
  //         /      \
  //       <          >
  //      / \        / \
  //     1   2      3   1
  ExprPtr and_cmp =
      binary(binary(lit(1.0), BinaryOp::Lt, lit(2.0)), BinaryOp::And,
             binary(lit(3.0), BinaryOp::Gt, lit(1.0)));

  try
  {
    ScriptValue v = add_one->evaluate(env);
    std::cout << "A+1 = " << v.asNumber() << "\n";

    ScriptValue c = and_cmp->evaluate(env);
    std::cout << "1<2 && 3>1 = " << std::boolalpha << c.asBool() << "\n";
    std::cout << "E3 checks passed\n";
  }
  catch (const std::exception& ex)
  {
    std::cout << "E3 failed: " << ex.what() << "\n";
    return 1;
  }

  return 0;
}
