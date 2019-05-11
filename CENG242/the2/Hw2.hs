module Hw2 where

data ASTResult = ASTError String | ASTJust (String, String, Int) deriving (Show, Read)
data ASTDatum = ASTSimpleDatum String | ASTLetDatum String deriving (Show, Read)
data AST = EmptyAST | ASTNode ASTDatum AST AST deriving (Show, Read)

isNumber :: String -> Bool
eagerEvaluation :: AST -> ASTResult
normalEvaluation :: AST -> ASTResult
-- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
-- IMPLEMENT isNumber, eagerEvaluation and normalEvaluation FUNCTIONS ACCORDING TO GIVEN SIGNATURES -- 

countSteps::Expression -> Int
countSteps (Num a)               = 0
countSteps (Str a)               = 0
countSteps (Var a)               = 0
countSteps (Let var left right)  = 0 + countSteps left + countSteps right
countSteps (Binary left o right) = 1 + countSteps left + countSteps right
countSteps (Unary o a)           = 1 + countSteps a

data Operation = Len
               | Neg
               | Plus
               | Times
               | Cat deriving (Show, Read)

data Expression = Num String
                | Str String
                | Var String
                | Unary Operation Expression
                | Binary Expression Operation Expression
                | Let String Expression Expression deriving (Show, Read)

type SymT = [(String, ASTResult)]


toExpr :: AST -> Expression
toExpr (ASTNode (ASTSimpleDatum a) EmptyAST EmptyAST) = Var a

toExpr (ASTNode (ASTSimpleDatum a) left EmptyAST)
                          | a == "num" = Num value
                          | a == "str" = Str value where (ASTNode (ASTSimpleDatum value) _ _) = left

toExpr (ASTNode (ASTSimpleDatum a) left right)
                          | a == "plus"   = Binary (toExpr left) Plus (toExpr right)
                          | a == "times"  = Binary (toExpr left) Times (toExpr right)
                          | a == "cat"    = Binary (toExpr left) Cat (toExpr right)
                          | a == "len"    = Unary Len (toExpr left)
                          | a == "negate" = Unary Neg (toExpr left)

toExpr (ASTNode (ASTLetDatum a) left right) = Let a (toExpr left) (toExpr right)


f :: SymT -> Expression -> ASTResult
f sym (Num a) = case isNumber(a) of
                                      True  -> ASTJust (a, "num", 0)
                                      False -> ASTError ("the value '"++a++"' is not a number!")
f sym (Str a) = ASTJust (a, "str", 0)
f sym (Var x) = ASTJust (res, tip, steps) where Just (ASTJust (res, tip, steps)) = lookup x sym
f sym (Let var left right) = case f sym left of
                                      ASTError err -> ASTError err
                                      ASTJust (a, b, c) -> f ((var, ASTJust (a, b, c)):sym) right
f sym (Unary Len a) = case f sym a of
                    ASTError err -> ASTError err
                    ASTJust (ares, atip, asteps) -> if atip == "str"
                                      then ASTJust  (show(length ares), "num", asteps+1)
                                      else ASTError ("len operation is not defined on " ++ atip ++ "!")
f sym (Unary Neg a) = case f sym a of
                    ASTError err -> ASTError err
                    ASTJust (ares, atip, asteps) -> if atip == "num"
                                      then ASTJust  (show((read ares::Int) * (-1)), "num", asteps+1)
                                      else ASTError ("negate operation is not defined on " ++ atip ++ "!")
f sym (Binary a Plus b) = case f sym a of
                    ASTError err -> ASTError err
                    ASTJust (ares, atip, asteps) -> case f sym b of
                                    ASTError err -> ASTError err
                                    ASTJust (bres, btip, bsteps) -> if atip == btip && btip == "num"
                                      then ASTJust  (show((read ares::Int) + (read bres::Int)), "num", asteps+bsteps+1)
                                      else ASTError ("plus operation is not defined between " ++ atip ++ " and " ++ btip ++ "!")
f sym (Binary a Times b) = case f sym a of
                    ASTError err -> ASTError err
                    ASTJust (ares, atip, asteps) -> case f sym b of
                                    ASTError err -> ASTError err
                                    ASTJust (bres, btip, bsteps) -> if atip == btip && btip == "num"
                                      then ASTJust  (show((read ares::Int) * (read bres::Int)), "num", asteps+bsteps+1)
                                      else ASTError ("times operation is not defined between " ++ atip ++ " and " ++ btip ++ "!")
f sym (Binary a Cat b) = case f sym a of
                    ASTError err -> ASTError err
                    ASTJust (ares, atip, asteps) -> case f sym b of
                                    ASTError err -> ASTError err
                                    ASTJust (bres, btip, bsteps) -> if atip == btip && btip == "str"
                                      then ASTJust  (ares ++ bres, "str", asteps+bsteps+1)
                                      else ASTError ("cat operation is not defined between " ++ atip ++ " and " ++ btip ++ "!")

--           error sıralaması

isNumber a = case reads a :: [(Int, String)] of
           [(res, "")] -> show res == a
           _           -> False

normalEvaluation tree = f [] (toExpr tree)

eagerEvaluation tree = case f [] x of
                  ASTError err      -> ASTError err
                  ASTJust (a, b, c) -> ASTJust (a, b, countSteps(x))
                  where x = toExpr tree