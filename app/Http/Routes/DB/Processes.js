/* 
 * This file has the routes used for testing without AAA but with Database
 */
// Method names according to the Google Cloud API Naming Conventions https://cloud.google.com/apis/design/naming_convention
const Route = use('Route')
// PROCESSES COLLECTION MANAGEMENT:
// MetadataList: get a list of processes' metadata 
Route.get('/db/processes/metadata', 'ProcessesDBController.MetadataList')
// MetadataItem: get a list of a process' metadata
Route.get('/db/processes/metadata/:id', 'ProcessesDBController.MetadataItem')
// GetList: get a list of processes
Route.get('/db/processes', 'ProcessesDBController.GetList')
// GetItem: get a process
Route.get('/db/processes/:id', 'ProcessesDBController.GetItem')
// CreateItem: create a process
Route.post('/db/processes', 'ProcessesDBController.CreateItem')
// DeleteItem: Deletes a process
Route.delete('/db/processes/:id', 'ProcessesDBController.DeleteItem')
// EmptyCollection: Deletes all processes in a collection for which the user is admin
Route.delete('/db/processes', 'ProcessesDBController.EmptyCollection')
