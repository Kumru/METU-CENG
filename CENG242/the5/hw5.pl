%    Y. CAHIT YILDIRIM / 2310647 / PROGRAMMING LANGUAGE CONCEPTS / HW5 / 2018 - 2019

:- module(hw5, [catomic_number/2, ion/2, molecule/2]).
:- [catoms].

% ---  MEMOIZATION FOR PERFORMANCE PURPOSES  ---
:- table catomic_number/2.
:- table ion/2.


%    -------------------------------------------------------------------------------


% ---  CATOMIC NUMBER FINDING  ---
catomic_number(NAME, CATOMIC_NUMBER)
    :- catom(NAME, _, _, LIST), sum_list(LIST, CATOMIC_NUMBER).

% --- ION CHARGE FINDING  ---
ion(NAME, B) :- catom(NAME, _, _, A), last(A, B1), (B1 < 5 -> B is B1; B is (B1-8)).

% ---  MOLECULE FINDING  ---
molecule(CATOM_LIST, TOTAL_CATOMIC_NUMBER)
    :- molecule(CATOM_LIST, TOTAL_CATOMIC_NUMBER, 0, 0).

molecule([], 0, _, 0).
molecule([H|T], SUM, PREV, CHARGE) :-
                                catomic_number(H, N),
                                    N>=PREV,
                                    NS is SUM-N,
                                    NS >= 0,
                                    ion(H, K),
                                    NC is CHARGE+K,
                                    molecule(T, NS, N, NC).

