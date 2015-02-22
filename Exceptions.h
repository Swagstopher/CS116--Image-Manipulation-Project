#pragma once
#include <string>

/**
 * Base class for exceptions. Contains a string explaining the cause of the 
 * exception. Classes should throw a more descriptive subclass of exception,
 * rather than a naked Exception.
 */
class Exception {
private:
    std::string message; /// a message explaining the reason for the exception
public:
    /**
     * Constructs an exception with the given explanatory message
     * @param message a message explaining why the exception occurred
     */
    Exception(std::string message) : message(message) { }
    /**
     * Gets the message explaining the exception
     * @return a string containing the reason for the exception
     */
    std::string getMessage() {
        return message;
    }    
};

/**
 * This exception should be thrown when an illegal argument (such as a null
 * pointer, negative number, or anything else that violates the preconditions)
 * is passed to a function or constructor.
 */
class IllegalArgumentException : public Exception {
public:
    /**
     * Constructs an exception with the given explanatory message
     * @param message a message explaining why the exception occurred
     */
    IllegalArgumentException(std::string message) : Exception(message) { }
};

/**
 * This exception should be thrown when an index passed to a function is out of
 * the bounds of the allowed indices. For example, the x index 70 is out of 
 * bounds for an image which only has a width of 50.
 */
class IndexOutOfBoundsException : public IllegalArgumentException {
public:
    /**
     * Constructs an exception with the given explanatory message
     * @param message a message explaining why the exception occurred
     */
    IndexOutOfBoundsException(std::string message) : IllegalArgumentException(message) {}
};

/**
 * This exception should be thrown when a file or file stream is in an invalid
 * state. For example, if the filename passed to a function that loads a bitmap
 * is of a text file, then the loading cannot continue and an exception must
 * be thrown.
 */
class FileException : public Exception {
private:
    std::string filename; /// the name of the file involved in the exception
public:
    /**
     * Constructs an exception with the given explanatory message
     * @param filename the name of the file involved in the exception
     * @param message a message explaining why the exception occurred
     */
    FileException(std::string filename, std::string message = "") 
            : Exception(message), filename(filename){ }
    /**
     * Gets the name of the file involved in the exception
     * @return the filename involved in the exception
     */        
    std::string getFilename() {
        return filename;
    }
};
