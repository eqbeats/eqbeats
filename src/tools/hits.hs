import Data.Bits
import qualified Data.ByteString.Lazy as B
import qualified Data.ByteString.Lazy.Char8 as B8
import Data.Word
import System.Environment

redis :: [B.ByteString] -> B.ByteString
redis args = B.concat . map (flip B.append (B8.pack "\r\n")) $
    B8.pack ('*' : show (length args)) : concatMap format args
  where format a = [ B8.pack ('$' : show (B.length a)), a ]

build :: B.ByteString -> Word32
build s = let [a, b, c, d] = B.unpack s in f d `shiftL` 24 + f c `shiftL` 16 + f b `shiftL` 8 + f a
  where f = fromIntegral

parse :: B.ByteString -> [Word32]
parse s | B.length s >= 4 = let (n, r) = B.splitAt 4 s in build n : parse r
parse _ = []

command n 0 = B.empty
command n h = redis $ map B8.pack ["SET", "stat:track:" ++ show n ++ ":trackView", show h]

main = getArgs >>= \[f] -> B.readFile f >>= mapM_ B.putStr . zipWith command [0..] . parse
