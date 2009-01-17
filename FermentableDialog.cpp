/*
 * FermentableDialog.cpp is part of Brewtarget, and is Copyright Philip G. Lee
 * (rocketman768@gmail.com), 2009.
 *
 * Brewtarget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Brewtarget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QWidget>
#include <QDialog>
#include "FermentableDialog.h"
#include "observable.h"
#include "database.h"
#include "recipe.h"
#include "MainWindow.h"
#include "fermentable.h"

FermentableDialog::FermentableDialog(MainWindow* parent)
        : QDialog(parent)
{
   setupUi(this);
   mainWindow = parent;
   dbObs = 0;
   numFerms = 0;

   connect( pushButton_addToRecipe, SIGNAL( clicked() ), this, SLOT( addFermentable() ) );
}

void FermentableDialog::notify(Observable *notifier)
{
   if( notifier != dbObs )
      return;

   if( numFerms != dbObs->getNumFermentables() )
   {
      fermentableTableWidget->getModel()->removeAll();
      populateTable();
   }
}

void FermentableDialog::startObservingDB()
{
   dbObs = Database::getDatabase();
   populateTable();
}

void FermentableDialog::populateTable()
{
   unsigned int i;

   if( ! Database::isInitialized() )
      return;

   numFerms = dbObs->getNumFermentables();
   for( i = 0; i < numFerms; ++i )
      fermentableTableWidget->getModel()->addFermentable(dbObs->getFermentable(i));
}

void FermentableDialog::addFermentable()
{
   QModelIndexList selected = fermentableTableWidget->selectedIndexes();
   int row, size, i;

   size = selected.size();
   if( size == 0 )
      return;

   // Make sure only one row is selected.
   row = selected[0].row();
   for( i = 1; i < size; ++i )
   {
      if( selected[i].row() != row )
         return;
   }

   Fermentable *ferm = fermentableTableWidget->getModel()->getFermentable(row);
   mainWindow->addFermentableToRecipe(new Fermentable(*ferm) ); // Need to add a copy so we don't change the database.
}
