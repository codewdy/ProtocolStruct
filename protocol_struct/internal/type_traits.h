#ifndef PROTOCOL_STRUCT_INTERNAL_TYPE_TRAITS_H_
#define PROTOCOL_STRUCT_INTERNAL_TYPE_TRAITS_H_

#include <type_traits>

namespace protocol_struct {
namespace internal {
namespace type_traits {

template <typename Thead, typename Ttail>
struct TypeList {
  using HeadType = Thead;
  using TailType = Ttail;
};

template <typename Tlhs, typename Trhs>
struct TypeMerger {
  using T = void;
};

template <typename Tlhs_head, typename Tlhs_tail,
          typename Trhs, typename Tmerge>
struct TypeConcatMergeHelper {
  using T = TypeList<Tlhs_head, Tmerge>;
};

template <typename Tlhs_head, typename Tlhs_tail, typename Trhs>
struct TypeConcatMergeHelper<Tlhs_head, Tlhs_tail, Trhs, void> {
  using T = TypeList<TypeList<Tlhs_head, Tlhs_tail>, Trhs>;
};

template <typename Tlhs, typename Trhs>
struct TypeConcat;

template <typename Tlhs, typename Trhs_head, typename Trhs_tail>
struct TypeConcat<Tlhs, TypeList<Trhs_head, Trhs_tail>> {
  using T = TypeList<typename TypeConcat<Tlhs, Trhs_head>::T, Trhs_tail>;
};

template <typename Tlhs, typename Trhs_tail>
struct TypeConcat<Tlhs, TypeList<void, Trhs_tail>> {
  using T = typename TypeConcatMergeHelper<
      typename Tlhs::HeadType,
      typename Tlhs::TailType,
      Trhs_tail,
      typename TypeMerger<typename Tlhs::TailType, Trhs_tail>::T>::T;
};

template <typename... Targs>
struct TypeConcatN;

template <typename Tlhs, typename Trhs, typename... Targs>
struct TypeConcatN<Tlhs, Trhs, Targs...> {
  using T = typename TypeConcatN<
      typename TypeConcat<Tlhs, Trhs>::T, Targs...>::T;
};

template <typename Tone>
struct TypeConcatN<Tone> {
  using T = Tone;
};

template <typename Tlst, typename Top>
struct ForEach;

template <typename Tlhs, typename Trhs, typename Top>
struct ForEach<TypeList<Tlhs, Trhs>, Top> {
#if __cplusplus >= 201703L
  using T = TypeList<typename ForEach<Tlhs, Top>::T,
                     typename std::invoke_result<Top, Trhs>::type>;
#else
  using T = TypeList<typename ForEach<Tlhs, Top>::T,
                     typename std::result_of<Top(Trhs)>::type>;
#endif
};

template <typename Top>
struct ForEach<void, Top> {
  using T = void;
};

}  // namespace type_traits
}  // namespace internal
}  // namespace protocol_struct

#endif  // PROTOCOL_STRUCT_INTERNAL_TYPE_TRAITS_H_
