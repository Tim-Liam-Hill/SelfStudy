

Normally in React you would pass information to child components via props and have a common parent manage and hold of the state necessary for its children. The problem is that this becomes very tedious to do as complexity grows and you runinto prop drilling: having to pass the same prop/s through a number of intermediary components who don't need them for the final child that does.

Contexts solve this problem.

In a way, contexts let children ask for info from components higher up the tree.

# How to use 

1. Create a context 
2. Use that context in component that needs the data.
3. Provide context from a parent that will also specify the data.

When we create a context, we provide only the default value for when that 
context is used!!!! 

```
import { createContext } from 'react';

export const LevelContext = createContext(1);

```

[If you don’t provide the context, React will use the default value you’ve specified in the previous step](https://react.dev/learn/passing-data-deeply-with-context). So the default value is only used when you don't specify a provider it seems. 

Think of things in terms of a tree!!! 
- Using a context means 'go up the tree till you find that context'
- As such you can have multiple components provide the same context but only the nearest parent's context applies (TODO: test, but that seems to be what happens [per the documentation](https://react.dev/learn/passing-data-deeply-with-context) -> "nested components can override context.")

Contexts are separate from one another and don't interact or override each other (unless nested components do the overriding).

You can use state with context to ensure that all components below the provider component get updated when a state is updated. 

# The Rerendering problem

Contexts can lead to problems.

- Suppose you have a large context at the root of your app 
- Many smaller intermediary and leaf components use this context 
- One component makes a small change to some state variable in the context 
- Any other component that uses that context will rerender even if the part of the context used by that component wasn't changed.

Put another way, [https://www.developerway.com/posts/react-re-renders-guide#part2.3](https://www.developerway.com/posts/react-re-renders-guide#part2.3)

