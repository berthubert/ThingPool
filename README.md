# ThingPool
Minimalistic, easy to use, generic object pool.

(C) 2024 Bert Hubert <bert@hubertnet.nl> - MIT License

Often you have an class (or Thing) that a single thread can use exclusively,
but which has some cost to create and tear down. The canonical example is
a database connection.

From ThingPool you can request a Thing, and if need be it will instantiate a
fresh instance for you. Or, in the common case, it will provide you with an
instance it made earlier.

Syntax:

ThingPool<SQLiteWriter> tp("tk.sqlite3", SQLWFlag::ReadOnly);

This says you want objects of type SQLiteWriter, and that they should be
created as: new SQLiteWriter("tk.sqlite3", SQLWFlag::ReadOnly);

Requesting an instance goes like this:

auto lease = tp.getLease();
lease->queryT("select count(1) from Document");

As long as 'lease' is alive, the instance is all yours. Once lease goes out of
scope, the object is returned to the pool. The -> syntax allows you to call
methods on SQLiteWriter.

Many threads can use getLease() at the same time, and returns are of course
also threadsafe.

If you allow ThreadPool to go out of scope (or if you destroy it) while there
are still active leases, this will throw an exception and likely kill your
process. Don't do this. There is likely no better robust way to deal with this
situation.

And that's it. Enjoy, feedback is welcome on bert@hubertnet.nl !

