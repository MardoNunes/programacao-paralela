#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>

//ALGORTIMO SSP - SHORTEST SUPERSTRING PROBLEM
//Dado um conjunto de strings, encontrar a menor superstring que contém todas as strings do conjunto como substrings
//Ex: Dado o conjunto {"ATEND", "NDA", "DADO"}, a menor superstring é "ATENDADO"
//SSP é um problema NP-Dificil e o algoritmo abaixo é totalmento sequencial

#define standard_input  std::cin
#define standard_output std::cout

using Boolean = bool ;
using Size    = std::size_t ;
using String  = std::string ;

using InStream  = std::istream ;
using OutStream = std::ostream ;

template <typename T, typename U>
using Pair = std::pair <T, U> ;

template <typename T, typename C = std::less <T>>
using Set = std::set <T> ;

template <typename T>
using SizeType = typename T :: size_type ;
//retorna o tamanho do conjunto
template <typename C> inline auto
size (const C& x) -> SizeType <C>
{
    return x.size () ;
}
//verifica se o conjunto tem pelo menos dois elementos
template <typename C> inline auto
at_least_two_elements_in (const C& c) -> Boolean
{
    return size (c) > SizeType <C> (1) ;
}
//retorna o primeiro elemento do conjunto
template <typename T> inline auto
first_element (const Set <T>& x) -> T
{
    return *(x.begin ()) ;
}
//retorna o segundo elemento do conjunto
template <typename T> inline auto
second_element (const Set <T>& x) -> T
{
    return *(std::next (x.begin ())) ;
}
//remove um elemento do conjunto
template <typename T> inline auto
remove (Set <T>& x, const T& e) -> Set <T>&
{
    x.erase (e) ;
    return x ;
}
//insere um elemento no conjunto
template <typename T> inline auto
push (Set <T>& x, const T& e) -> Set <T>&
{
    x.insert (e) ;
    return x ;
}
//verifica se o conjunto esta vazio
template <typename C> inline auto
empty (const C& x) -> Boolean
{
    return x.empty () ;
}
//verifica se a string a e prefixo da string b
// Ex: is_prefix("TEN", "TENDA") retorna true.
Boolean is_prefix (const String& a, const String& b)
{
    if (size (a) > size (b))    //se o tamanho de a for maior que o de b, não pode ser prefixo
        return false ;
    if ( !
            ( std::mismatch     //compara os elementos de a com os de b, se mismatch encontra o primeiro elemento diferente
                ( a.begin ()      
                , a.end   ()
                , b.begin () )
                    .first == a.end () ) )
        return false ;
    return true ;
}

//retorna o sufixo da string x a partir da posição i
inline auto
suffix_from_position (const String& x, SizeType <String> i) -> String
{
    return x.substr (i) ;
}

//remove os n primeiros caracteres da string x
inline auto
remove_prefix (const String& x, SizeType <String> n) -> String
{
    if (size (x) > n)
        return suffix_from_position (x, n) ;
    return x ;
}

//Gera um conjunto com todos os sufixos da string x
// Ex: all_suffixes("TENDA") retorna {"ENDA", "NDA", "DA", "A"}
auto
all_suffixes (const String& x) -> Set <String>
{
    Set <String> ss ;
    SizeType <String> n = size (x) ;
    while (-- n) {
        ss.insert (x.substr (n)) ;
    }
    return ss ;
}


//Encontra o maior sufixo de a que é prefixo de b
// Ex: commom_suffix_and_prefix("TENDA", "DADO") retorna "DA
auto
commom_suffix_and_prefix (const String& a, const String& b) -> String
{
    if (empty (a)) return "" ;
    if (empty (b)) return "" ;
    String x = "" ;
    //percorre todos os sufixos de a, verificando se é prefixo de b e se é maior que o sufixo atual
    for (const String& s : all_suffixes (a)) {
        if (is_prefix (s, b) && size (s) > size (x)) {
            x = s ;
        }
    }
    return x ;
}

//retorna o tamanho do maior sufixo de s que é prefixo de t, maior sobreposição entre s e t
//Ex: overlap_value("TENDA", "DADO") retorna 2
inline auto
overlap_value (const String& s, const String& t) -> SizeType <String>
{
    return size (commom_suffix_and_prefix (s, t)) ;
}

//realiza a sobreposição entre s e t
//Ex: overlap("ATEND", "NDA") retorna "ATENDA". E não "ATENDNDA"
auto
overlap (const String& s, const String& t) -> String
{
    String c = commom_suffix_and_prefix (s, t) ;    //maior sufixo de s que é prefixo de t
    return s + remove_prefix (t, size (c)) ;    //concatena s com t sem o prefixo c
}

//remove os dois elementos do par do conjunto e insere a sobreposição deles
inline auto
pop_two_elements_and_push_overlap
        (Set <String>& ss, const Pair <String, String>& p) -> Set <String>&
{
    ss = remove (ss, p.first)  ;
    ss = remove (ss, p.second) ;
    ss = push   (ss, overlap (p.first, p.second)) ;
    return ss ;
}

//gera um conjunto com todos os pares distintos de strings do conjunto
// Ex: all_distinct_pairs({"A", "B", "C"}) retorna {("A", "B"), ("A", "C"), ("B", "A"), ("B", "C"), ("C", "A"), ("C", "B")}
auto
all_distinct_pairs (const Set <String>& ss) -> Set <Pair <String, String>>
{
    Set <Pair <String, String>> x ;
    for (const String& s1 : ss) {
        for (const String& s2 : ss) {
            if (s1 != s2) x.insert (make_pair (s1, s2)) ;
        }
    }
    return x ;
}

//retorna o par de strings com maior overlap value, ou seja, maior sobreposição
//Está é uma escolha gulosa, pois escolhe o par com maior sobreposição a cada iteração
auto
highest_overlap_value
        (const Set <Pair <String, String>>& sp) -> Pair <String, String>
{
    Pair <String, String> x = first_element (sp) ;  //inicializa x com o primeiro par do conjunto
    for (const Pair <String, String>& p : sp) {
        if ( overlap_value (p.first, p.second)  
                > overlap_value (x.first, x.second) ) //se o par p tem maior sobreposição que o par x, atualiza x
        {
            x = p ; //atualiza x
        }
    }
    return x ;
}

//retorna o par de strings com maior overlap value do conjunto de strings ss, ou seja, maior sobreposição
//é uma junçõa das duas funções anteriores
auto
pair_of_strings_with_highest_overlap_value
        (const Set <String>& ss) -> Pair <String, String>
{
    return highest_overlap_value (all_distinct_pairs (ss)) ;
}

//função principal que encontra a menor superstring
auto
shortest_superstring (Set <String> t) -> String
{
    if (empty (t)) return "" ;  //se o conjunto estiver vazio, retorna string vazia
    while (at_least_two_elements_in (t)) {  //enquanto o conjunto tiver pelo menos dois elementos
        //1. encontra o par de strings com maior sobreposição
        //2. remove os dois elementos do par do conjunto e insere a sobreposição deles
        t = pop_two_elements_and_push_overlap
            ( t
            , pair_of_strings_with_highest_overlap_value (t) ) ;
    }
    //quando o conjunto tiver apenas um elemento, retorna esse elemento, que é a menor superstring
    return first_element (t) ;
}

//funções de entrada e saída
inline auto
write_string_and_break_line (OutStream& out, String s) -> void
{
    out << s << std::endl ;
}

inline auto
read_size (InStream& in) -> Size
{
    Size n ;
    in >>  n ;
    return n ;
}

inline auto
read_string (InStream& in) -> String
{
    String s ;
    in >>  s ;
    return s ;
}

auto
read_strings_from_standard_input () -> Set <String>
{
    using N = SizeType <Set <String>> ; //cria um tipo N para o tamanho do conjunto de strings
    Set <String> x ;
    N n = N (read_size (standard_input)) ;  //le o tamanho do conjunto
    while (n --) x.insert (read_string (standard_input)) ;  //le as strings e insere no conjunto
    return x ;
}

inline auto
write_string_to_standard_ouput (const String& s) -> void
{
    write_string_and_break_line (standard_output, s) ;
}

auto
main (int argc, char const* argv[]) -> int
{
    Set <String> ss = read_strings_from_standard_input () ;
    write_string_to_standard_ouput (shortest_superstring (ss)) ;
    return 0 ;
}

