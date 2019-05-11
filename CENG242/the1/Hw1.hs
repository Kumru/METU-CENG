module Hw1 where

  type Mapping = [(String, String, String)]
  data AST = EmptyAST | ASTNode String AST AST deriving (Show, Read)
  
  writeExpression :: (AST, Mapping) -> String
  evaluateAST :: (AST, Mapping) -> (AST, String)
  -- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
  -- IMPLEMENT writeExpression and evaluateAST FUNCTION ACCORDING TO GIVEN SIGNATURES -- 
  
  ------------------------- writeExpression ------------------------

  inorder :: AST -> [Char]
  
  inorder EmptyAST = []
  inorder (ASTNode a left EmptyAST) | a == "num" = inorder left
                                    | a == "str" = "\"" ++ inorder left ++ "\""
                                    | a == "len" = "(length " ++ inorder left ++ ")"
                                    | a == "negate" = "(" ++ "-" ++ inorder left ++ ")"
  inorder (ASTNode a left right)    | a == "plus" = "(" ++ inorder left ++ "+" ++ inorder right ++ ")"
                                    | a == "times" = "(" ++ inorder left ++ "*" ++ inorder right ++ ")"
                                    | a == "cat" = "(" ++ inorder left ++ "++" ++ inorder right ++ ")"
                                    | otherwise = a

  mLT ((a, b, c) : []) | b == "num" = a ++ "=" ++ c
                       | b == "str" = a ++ "=" ++ "\"" ++ c ++ "\""
  mLT ((a, b, c) : xs) | b == "num" = a ++ "=" ++ c ++ ";" ++ mLT xs
                       | b == "str" = a ++ "=" ++ "\"" ++ c ++ "\"" ++ ";" ++ mLT xs
  
  writeExpression (tree, []) = inorder tree
  writeExpression (tree, mapList) = "let " ++ mLT mapList ++ " in " ++ inorder tree
  
  ---------------------- evaluateExpression ------------------------
  
  data Operation = Len
                 | Neg
                 | Plus
                 | Times
                 | Cat deriving (Show, Read)
  
  data Expression = Num String
                  | Str String
                  | Unary Operation Expression
                  | Binary Expression Operation Expression deriving (Show, Read)
  
  eval::Expression -> String
  eval (Num a)            = a
  eval (Str a)            = a
  eval (Unary Len m)      = show (length (eval m))
  eval (Unary Neg m)      = show ((read (eval m)::Int)*(-1))
  eval (Binary m Plus n)  = show ((read (eval m)::Int) + (read (eval n)::Int))
  eval (Binary m Times n) = show ((read (eval m)::Int) * (read (eval n)::Int))
  eval (Binary m Cat n)   = eval m ++ eval n

  toExpr::AST -> Expression
  toExpr (ASTNode a left right) | a == "num"    = Num value
                                | a == "str"    = Str value
                                | a == "plus"   = Binary (toExpr left) Plus (toExpr right)
                                | a == "times"  = Binary (toExpr left) Times (toExpr right)
                                | a == "cat"    = Binary (toExpr left) Cat (toExpr right)
                                | a == "len"    = Unary Len (toExpr left)
                                | a == "negate" = Unary Neg (toExpr left)
                              where ASTNode value EmptyAST EmptyAST = left

  replacer::AST -> (String, String, String) -> AST
  replacer EmptyAST                           (_, _, _) = EmptyAST
  replacer tree@(ASTNode a EmptyAST EmptyAST) (x, y, z) | a == x = ASTNode y (ASTNode z EmptyAST EmptyAST) EmptyAST
                                                        | otherwise = tree
  replacer (ASTNode a left right)             (x, y, z) = (ASTNode a (replacer left (x, y, z)) (replacer right (x, y, z)))

  autoReplacer::(AST,Mapping) -> AST
  autoReplacer (tree, [])     = tree
  autoReplacer (tree, (x:xs)) = autoReplacer ((replacer tree x), xs)

  evaluateAST (tree, mapping) = (resultTree, (eval (toExpr resultTree)))
                              where resultTree = autoReplacer (tree, mapping)