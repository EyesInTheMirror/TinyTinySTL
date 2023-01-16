//
// Created by 镜瞳 on 2022/12/24.
//

#ifndef TINYTINYSTL_ITERATOR_H
#define TINYTINYSTL_ITERATOR_H
#include <cstddef>

namespace tinystl{
    //迭代器的category设计
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag{};
    struct bidirection_iterator_tag : public forward_iterator_tag{};
    struct random_access_iterator_tag : public bidirection_iterator_tag{};

    //迭代器设计模版，确保每一个新设计的迭代器都继承它
    template <typename Category, typename T, typename Distance = ptrdiff_t ,
            typename Pointer = T*, typename Reference = T&>
    struct iterator{
        using iterator_category = Category;
        using value_type = T;
        using pointer = Pointer;
        using reference = Reference;
        using difference_type = Distance;
    };

    //迭代器类别萃取器
//    template <typename Iterator>
//    struct iterator_traits{
//        using iterator_category = typename Iterator::iterator_category;
//        using value_type = typename Iterator::value_type;
//        using pointer = typename Iterator::pointer;
//        using reference = typename Iterator::reference;
//        using difference_type = typename Iterator::difference_type;
//    };
    template <class T>
    struct has_iterator_cat
    {
    private:
        struct two { char a; char b; };
        template <class U> static two test(...);
        template <class U> static char test(typename U::iterator_category* = 0);
        // 运用了SFINAE技巧，若有iterator_category属性，则会优先匹配到
    public:
        static const bool value = sizeof(test<T>(0)) == sizeof(char);
    };

    template <typename Iterator, bool>
    struct iterator_traits_impl {};

    template <typename Iterator>
    struct iterator_traits_impl<Iterator, true>{
        using iterator_category = typename Iterator::iterator_category;
        using value_type = typename Iterator::value_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
        using difference_type = typename Iterator::difference_type;
    };

    template <typename Iterator, bool>
    struct iterator_traits_helper {};

    template <typename Iterator>
    struct iterator_traits_helper<Iterator, true>
            : public iterator_traits_impl<Iterator,
            std::is_convertible<typename Iterator::iterator_catecory, input_iterator_tag>::value ||
            std::is_convertible<typename Iterator::iterator_catecory, output_iterator_tag>::value>
    {};
    template <typename Iterator>
    struct iterator_traits
        : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value>
        {};

    //对于原始指针的偏特化版本
    template <typename T>
    struct iterator_traits<T*>{
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = ptrdiff_t;
    };

    template <typename T>
    struct iterator_traits<const T*>{
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = ptrdiff_t;
    };

    //萃取迭代器的各种信息，生成一个临时对象，触发函数的参数推导，利用重载机制提高某些算法的效率
    template <typename Iterator>
    typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&){
        using category = typename iterator_traits<Iterator>::iterator_category;
        return category();
    }

    template <typename Iterator>
    typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&){
        using value_type = typename iterator_traits<Iterator>::value_type;
        return static_cast<value_type*>(0);
    }

    template <typename Iterator>
    typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&){
        using difference_type = typename iterator_traits<Iterator>::difference_type;
        return static_cast<difference_type*>(0);
    }

    //针对迭代器是否为随机读写迭代器，实现两个重载函数，对症下药确保尽可能高效
    template <typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    _distance(InputIterator first, InputIterator last, input_iterator_tag){
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while(first != last){
            first++;
            n++;
        }
        return n;
    }

    template <typename RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type
    _distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag){
        return last - first;
    }
    //对外的接口
    template <typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last){
        return _distance(first, last, iterator_category(first));
    }

    //针对迭代器类别实现三个重载函数，其中随机访问迭代器最直接，双向迭代器可以接受负值往回走
    template <typename InputIterator, typename Distance>
    inline void _advance(InputIterator& cur, Distance n, input_iterator_tag){
        while(n--) ++cur;
    }

    template <typename BidirectionalIterator, typename Distance>
    inline void _advance(BidirectionalIterator& cur, Distance n, bidirection_iterator_tag){
        if(n >= 0)
            while(n--) ++cur;
        else
            while(n++) --cur;
    }

    template <typename RandomAccessIterator, typename Distance>
    inline void _advance(RandomAccessIterator& cur, Distance n, random_access_iterator_tag){
        cur += n;
    }

    template<typename InputIterator, typename Distance>
    inline void advance(InputIterator& cur, Distance n){
        _advance(cur, n, iterator_category(cur));
    }




    template <class T, T v>
    struct m_integral_constant
    {
        static constexpr T value = v;
    };

    template <bool b>
    using m_bool_constant = m_integral_constant<bool, b>;

    typedef m_bool_constant<true>  m_true_type;
    typedef m_bool_constant<false> m_false_type;


    template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
    struct has_iterator_cat_of
            : public m_bool_constant<std::is_convertible<
                    typename iterator_traits<T>::iterator_category, U>::value>
    {
    };

    template <class T, class U>
    struct has_iterator_cat_of<T, U, false> : public m_false_type {};

    template <class Iter>
    struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag> {};
}

#endif //TINYTINYSTL_ITERATOR_H
