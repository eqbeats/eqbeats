import Control.Concurrent
import Control.Exception
import Control.Monad
import Network
import System.Directory
import System.Environment
import System.Exit
import System.IO
import System.Process

main :: IO ()
main = getArgs >>= \as -> case as of
    [cmd, sock] -> multiplex sock cmd
    _ -> hPutStrLn stderr "Usage: multiplex CMD SOCK" >> exitFailure

multiplex :: FilePath -> String -> IO ()
multiplex sock cmd = do
    lock <- newMVar ()
    let line h = hSetBuffering h LineBuffering
    (Just pin, Just pout, _, ph) <- createProcess (shell cmd)
        { std_in = CreatePipe, std_out = CreatePipe, std_err = Inherit }
    line pin >> line pout >> line stderr -- line buffering, line buffering everywhere
    bracket (listenOn (UnixSocket sock)) (clean sock ph) $ \server -> do
        _ <- forkIO . forever $ do
            (h, _, _) <- accept server
            line h
            forkIO $ serve h pin pout lock >> hClose h
        waitForProcess ph >>= exitWith

clean :: FilePath -> ProcessHandle -> Socket -> IO ()
clean sock p server = do
    sClose server
    removeFile sock
    terminateProcess p

clear :: Handle -> IO ()
clear h = hReady h >>= \r -> if r then hGetChar h >> clear h else return ()

serve :: Handle -> Handle -> Handle -> MVar () -> IO ()
serve h pin pout lock = forever $ do
    l <- hGetLine h >>= evaluate
    withMVar lock $ \() -> do
        clear pout
        hPutStrLn pin l
        hGetLine pout >>= hPutStrLn h
