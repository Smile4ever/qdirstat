/*
 *   File name:	CleanupCollection.h
 *   Summary:	Support classes for QDirStat
 *   License:   GPL V2 - See file LICENSE for details.
 *
 *   Author:	Stefan Hundhammer <Stefan.Hundhammer@gmx.de>
 */


#ifndef CleanupCollection_h
#define CleanupCollection_h


#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

#include "Cleanup.h"

// Forward declarations
class KActionCollection;


namespace QDirStat
{
    typedef QDict<Cleanup>		KCleanupDict;
    typedef QDictIterator<Cleanup>	KCleanupDictIterator;

    typedef QPtrList<Cleanup>		KCleanupList;
    typedef QPtrListIterator<Cleanup>	KCleanupListIterator;

    
    /**
     * Set of @ref Cleanup actions to be performed for @ref DirTree items,
     * consisting of a number of predefined and a number of user-defined
     * cleanups. The prime purpose of this is to make save/restore operations
     * with a number of cleanups easier. Thus, it provides a copy constructor,
     * an assignment operator and various methods to directly access individual
     * cleanups.
     *
     * @short QDirStat cleanup action collection
     **/

    class CleanupCollection: public QObject
    {
	Q_OBJECT

    public:

	/**
	 * Constructor.
	 *
	 * Most applications will want to pass KMainWindow::actionCollection()
	 * for 'actionCollection' so the menus and toolbars can be created
	 * using the XML UI description ('kdirstatui.rc' for QDirStat).
	 *
	 * All @ref Cleanup actions ever added to this collection will get
	 * this as their parent.
	 **/
	CleanupCollection( KActionCollection *	actionCollection = 0 );

	/**
	 * Copy Constructor.
	 *
	 * Makes a deep copy of this collection with 'actionCollection' set to
	 * 0 for all copied cleanups. Please note that since there is no
	 * complete copy constructor for @ref Cleanup, all restrictions to the
	 * @ref Cleanup copy constructor apply to the CleanupCollection, too:
	 * This copy constructor is intended for save/restore operations only,
	 * not for general use.  In particular, DO NOT connect an object thus
	 * constructed with signals. The results will be undefined (at best).
	 **/
	CleanupCollection( const CleanupCollection &src );

	/**
	 * Assignment operator.
	 *
	 * This operator has the same restrictions as the copy constructor:
	 * Just like the copy constructor, this is intended for save/restore
	 * operations, not for general use.
	 *
	 * For details, see the extensive comments in the source file.
	 **/
	CleanupCollection &	operator= ( const CleanupCollection &src );

	/**
	 * Destructor
	 **/
	virtual ~CleanupCollection();

	/**
	 * Add the standard cleanups to this collection.
	 **/
	void addStdCleanups();

	/**
	 * Add 'number' user-defined cleanups to this collection.
	 **/
	void addUserCleanups( int number );

	/**
	 * Add one single cleanup to this collection. The collection assumes
	 * ownerwhip of this cleanup - don't delete it!
	 **/
	void add( Cleanup *cleanup );

	/**
	 * Retrieve a cleanup by its ID (internal name).
	 * Returns 0 if there is no such cleanup.
	 **/
	Cleanup * cleanup( const QString & id );

	/**
	 * An alias to @ref cleanup() for convenience: Thus, you can use
	 * collection[ "cleanup_id" ] to access any particular cleanup.
	 **/
	Cleanup * operator[] ( const QString & id )
	    { return cleanup( id ); }
	
	/**
	 * Remove all cleanups from this collection.
	 **/
	void clear();

	/**
	 * Return (a shallow copy of) the internal cleanup list.
	 *
	 * Use this and a CleanupListIterator to iterate over all cleanups in
	 * this collection. Remember to keep the list until you no longer need
	 * the iterator! 
	 *
	 *	CleanupCollection *coll = ...
	 *	CleanupList cleanup_list = coll->cleanupList();
	 *	CleanupListIterator it( cleanup_list );
	 *
	 *	while ( *it )
	 *	{
	 *	    logDebug() << "Found cleanup " << *it << endl;
	 *	    ++it;
	 *	}
	 **/
	CleanupList cleanupList() const { return _cleanupList; }

	/**
	 * Return the number of cleanup actions in this collection.
	 **/
	int size() const { return _cleanupList.count(); }

	/**
	 * For internal use only: Returns the number to be assigned to the next
	 * user cleanup that may be added.
	 **/
	int nextUserCleanupNo() const { return _nextUserCleanupNo; }

    public slots:

        /**
	 * Emit the readConfig() signal for all cleanups.
	 **/
        void slotReadConfig();

	
    signals:
	
	/**
	 * Emitted when the currently selected item changes.
	 * 'item' may be 0 when the selection is cleared.
	 *
	 * Connect a view's selectionChanged() signal to this
	 * selectionChanged() signal to have the cleanup collection pass this
	 * signal to its cleanups.
	 **/
	void selectionChanged( FileInfo *item );

        /**
	 * Read collection for all cleanups.
	 **/
        void readConfig();

	/**
	 * Save configuration for all cleanups.
	 **/
	void saveConfig();

	/**
	 * Emitted at user activity, i.e. when the user executes a cleanup.
	 * This is intended for use together with a @ref KActivityTracker.
	 **/
	void userActivity( int points );


    protected slots:

        /**
	 * Connected to each cleanup's @ref executed() signal to track user
	 * activity. 
	 **/
	void cleanupExecuted();


    protected:

	/**
	 * Internal implementation of copy constructor and assignment operator:
	 * Make a deep copy of the collection.
	 **/
	void deepCopy( const CleanupCollection &src );

	
	// Data members
	
        KActionCollection *	_actionCollection;
	int			_nextUserCleanupNo;
	CleanupList		_cleanupList;
	CleanupDict		_cleanupDict;
    };
}	// namespace QDirStat


#endif // ifndef CleanupCollection_h


// EOF