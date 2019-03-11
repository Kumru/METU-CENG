module Hw1 where

  type Mapping = [(String, String, String)]
  data AST = EmptyAST | ASTNode String AST AST deriving (Show, Read)
  
  writeExpression :: (AST, Mapping) -> String
  evaluateAST :: (AST, Mapping) -> (AST, String)
  -- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
  -- IMPLEMENT writeExpression and evaluateAST FUNCTION ACCORDING TO GIVEN SIGNATURES -- 
  
  inorder :: AST -> [Char]
  
  inorder EmptyAST = []
  
  inorder (ASTNode a left EmptyAST) | a == "num" = inorder left
                                    | a == "str" = "\"" ++ inorder left ++ "\""
                                    | a == "len" = "(length " ++ inorder left ++ ")"
                                    | a == "negate" = "(" ++ "-" ++ inorder left ++ ")"
  
  inorder (ASTNode a left right) | a == "plus" = "(" ++ inorder left ++ "+" ++ inorder right ++ ")"
                                 | a == "times" = "(" ++ inorder left ++ "*" ++ inorder right ++ ")"
                                 | a == "cat" = "(" ++ inorder left ++ "++" ++ inorder right ++ ")"
                                 | otherwise = a
  
  
  
  mLT ((a, b, c) : []) | b == "num" = a ++ "=" ++ c
                       | b == "str" = a ++ "=" ++ "\"" ++ c ++ "\""
  --                     | otherwise = c
  
  mLT ((a, b, c) : xs) | b == "num" = a ++ "=" ++ c ++ ";" ++ mLT xs
                       | b == "str" = a ++ "=" ++ "\"" ++ c ++ "\"" ++ ";" ++ mLT xs
  --                     | otherwise = c
  
  
  
  writeExpression (tree, []) = inorder tree
  
  writeExpression (tree, mapList) = "let " ++ mLT mapList ++ " in " ++ inorder tree
  
  ------------------------------------------------------------------
  
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

  astToStr::AST -> String
  astToStr (ASTNode a EmptyAST EmptyAST) = a
  
  exec::AST -> Expression
  exec (ASTNode a left EmptyAST) | a == "num"    = Num (astToStr left)
                                 | a == "str"    = Str (astToStr left)

  exec (ASTNode a left right)    | a == "plus"   = Binary (exec left) Plus (exec right)
                                 | a == "times"  = Binary (exec left) Times (exec right)
                                 | a == "cat"    = Binary (exec left) Cat (exec right)
                                 | a == "len"    = Unary Len (exec left)
                                 | a == "negate" = Unary Neg (exec left)


  execToStr::AST -> String
  execToStr tree = eval (exec tree)

  replacer::AST -> (String, String, String) -> AST
  replacer EmptyAST (_, _, _)= EmptyAST
  replacer tree@(ASTNode a EmptyAST EmptyAST) (x, y, z) | a == x = ASTNode y (ASTNode z EmptyAST EmptyAST) EmptyAST
                                                        | otherwise = tree
  replacer (ASTNode a left right) (x, y, z) = (ASTNode a (replacer left (x, y, z)) (replacer right (x, y, z)))

  autoReplacer::(AST,Mapping) -> AST
  autoReplacer (tree, [])     = tree
  autoReplacer (tree, (x:xs)) = autoReplacer ((replacer tree x), xs)

  evaluateAST (tree, mapping) = (resultTree, (execToStr resultTree))
                              where resultTree = autoReplacer (tree, mapping)