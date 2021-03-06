#ifndef ATOMIC_SENTENCE_HPP
#define ATOMIC_SENTENCE_HPP
#include "term.hpp"
#include "named_parameter.hpp"
namespace first_order_logic
{
    DEFINE_ACTOR(equal);
    DEFINE_ACTOR(predicate);
    DEFINE_ACTOR(propositional_letter);
    struct atomic_sentence
    {
        enum class type { equal, predicate, propositional_letter };
        template< typename OS >
        OS friend operator << ( const OS &, const type & );
        struct internal
        {
            type atomic_sentence_type;
            std::string name;
            mutable std::string cache;
            std::vector< term > arguments;
            explicit internal(
                    type sentence_type,
                    const std::string & name,
                    const std::initializer_list< term > & r ) :
                atomic_sentence_type( sentence_type ),
                name( name ),
                arguments( r.begin( ), r.end( ) ) { }
            explicit internal(
                    type sentence_type,
                    const std::string & name,
                    const std::vector< term > & r ) :
                atomic_sentence_type( sentence_type ),
                name( name ),
                arguments( r.begin( ), r.end( ) ) { }
            explicit internal(
                    type sentence_type,
                    const std::initializer_list< term > & r ) :
                atomic_sentence_type( sentence_type ),
                arguments( r.begin( ), r.end( ) ) { }
            explicit internal(
                    type sentence_type,
                    const std::string & name ) :
                atomic_sentence_type( sentence_type ), name( name ) { }
        };
        bool operator < ( const atomic_sentence & as ) const
        { return static_cast< std::string >( * this ) < static_cast< std::string >( as ); }
        bool operator == ( const atomic_sentence & as ) const
        { return static_cast< std::string >( * this ) == static_cast< std::string >( as ); }
        bool operator != ( const atomic_sentence & as ) const
        { return static_cast< std::string >( * this ) != static_cast< std::string >( as ); }
        std::shared_ptr< internal > data;
        template< typename RET, typename ... T >
        RET type_restore_full( const T & ... t ) const
        {
            static_assert(
                        std::tuple_size< std::tuple< T ... > >::value == 3,
                        "should have three arguments" );
            return type_restore< RET >( t ..., error< RET >( ) );
        }
        template< typename RET, typename ... T >
        RET type_restore( const T & ... t ) const
        {
            return type_restore_inner< RET >(
                        extract< equal_actor_helper >(
                            t ...,
                            make_equal_actor(
                                std::get
                                <
                                    std::tuple_size
                                    <
                                        std::tuple< T ... >
                                    >::value -
                                    1
                                >( std::tie( t ... ) ) ) ),
                        extract< predicate_actor_helper >(
                            t ...,
                            make_predicate_actor(
                                std::get
                                <
                                    std::tuple_size
                                    <
                                        std::tuple< T ... >
                                    >::value -
                                    1
                                >( std::tie( t ... ) ) ) ),
                        extract< propositional_letter_actor_helper >(
                            t ...,
                            make_propositional_letter_actor(
                                std::get
                                <
                                    std::tuple_size
                                    <
                                        std::tuple< T ... >
                                    >::value -
                                    1
                                >( std::tie( t ... ) ) ) ) );
        }
        template< typename RET, typename T1, typename T2, typename T3 >
        RET type_restore_inner(
                const equal_actor< T1 > & equal_func,
                const predicate_actor< T2 > & predicate_func,
                const propositional_letter_actor< T3 > & propositional_letter_func ) const
        {
            switch ( (*this)->atomic_sentence_type )
            {
            case type::equal:
                return equal_func( (*this)->arguments[0], (*this)->arguments[1] );
            case type::predicate:
                return predicate_func( (*this)->name, (*this)->arguments );
            case type::propositional_letter:
                return propositional_letter_func( (*this)->name );
            }
            throw std::invalid_argument( "unknown enum type" );
        }
        const internal * operator -> ( ) const { return data.get( ); }
        explicit operator const std::string &( ) const
        {
            if ( ! (*this)->cache.empty( ) ) { return (*this)->cache; }
            return (*this)->cache =
                    type_restore_full< std::string >
                    (
                        make_equal_actor(
                            [&]( const term & l, const term & r )
                            {
                                return
                                        static_cast< std::string >( l ) +
                                        "=" +
                                        static_cast< std::string >( r );
                            } ),
                        make_predicate_actor(
                            [&]( const std::string & str, const std::vector< term > & vec )
                            {
                                std::string stack;
                                auto it = vec.begin( );
                                while ( it != vec.end( ) )
                                {
                                    if ( it != vec.begin( ) ) { stack += ", "; }
                                    stack += static_cast< std::string >( * it );
                                    ++it;
                                }
                                return str + "(" + stack + ")";
                            } ),
                        make_propositional_letter_actor(
                            [&]( const std::string & str ) { return str; } )
                    );
        }
        explicit atomic_sentence( type ty, const std::string & str, const std::vector< term > & ter ) :
            data( new internal( ty, str, ter ) ) { }
        explicit atomic_sentence( type ty, const std::initializer_list< term > & ter ) :
            data( new internal( ty, ter ) ) { }
        explicit atomic_sentence( type ty, const std::string & str ) :
            data( new internal( ty, str ) ) { }
    };

    template< typename OS >
    OS & operator << ( OS & os, const atomic_sentence & st )
    { return os << static_cast< std::string >( st ); }

    template< typename OS >
    OS & operator << ( OS & os, const atomic_sentence::type & st )
    {
        return os <<
                    ( st == atomic_sentence::type::equal ?
                        "equal" :
                    st == atomic_sentence::type::predicate ?
                        "predicate" :
                    st == atomic_sentence::type::propositional_letter ?
                        "propositional_letter" :
                    std::to_string( static_cast< long >( st ) ) );
    }
}
#endif // ATOMIC_SENTENCE_HPP
