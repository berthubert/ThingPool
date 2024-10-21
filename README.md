# ThingPool
Minimalistic, easy to use, generic object pool.

(C) 2024 Bert Hubert <bert@hubertnet.nl> - MIT License

Often you have a class (or Thing) that a single thread can use exclusively,
but which has some cost to create and tear down. The canonical example is
a database connection.

From ThingPool you can request a Thing, and if need be it will instantiate a
fresh instance for you. Or, in the common case, it will provide you with an
instance it made earlier.

Syntax:

```C++
ThingPool<SQLiteWriter> tp("tk.sqlite3", SQLWFlag::ReadOnly);
```

This says you want objects of type SQLiteWriter, and that they should be
created as: `new SQLiteWriter("tk.sqlite3", SQLWFlag::ReadOnly);`

Requesting an instance goes like this:

```C++
auto lease = tp.getLease();
lease->queryT("select count(1) from Document");
```

As long as 'lease' is alive, the instance is all yours. Once lease goes out of
scope, the object is returned to the pool. The -> syntax allows you to call
methods on SQLiteWriter. If you need a reference to your Thing, use get() (much
like smart pointers).

Many threads can use getLease() at the same time, and returns are of course
also threadsafe.

If you no longer need a lease, you can call its release() method to return it
to the pool. If you think the state of your object is bad, you can call the
abandon() method, which will delete the object and not return it to the pool.

If you allow ThingPool to go out of scope (or if you destroy it) while there
are still active leases, this will throw an exception and likely kill your
process. Don't do this. There is likely no better robust way to deal with this
situation.

# Including ThingPool
You can simply include the thingpool.hh file. There is a small Makefile in
this repository, and you can use that to build the testrunner to exercise
the code.

# That's it
Enjoy, feedback is welcome on bert@hubertnet.nl !

