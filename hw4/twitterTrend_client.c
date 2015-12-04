/* outline of this file (and its header)
 *
 * TODO: okay I think I want to add a "message" struct of some sort to handle the messages
 * 
 * main:
 *  put files to read in array (for ec) (not hard so might as well)
 * 	socket(): create socket
 *	handshake(): do two-way handshake with server
 *		: handshake includes the sending of message (#100) and message (#101)
 *	for each file to read: (for ec) (not hard so mgiht as well)
 *		report = createReportFile(): ...
 *		for each line in file:
 *			sendMessage(twitterTrendRequest): send linei request (#102) to server
 *			waitMessage(): wait for response
 *			error(reply): quit if reply was error message (#106)
 *			write(kwd reply): write kwd's of reply to this file's .report
 *	sendMessage(endOfRequest): send end of request message (#104)
 *
 */
