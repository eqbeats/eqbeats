<div id="api">
    <h2>API</h2>

    <h3>Quickstart</h3>
    <p>We have a read-only JSON/JSONP api to access user and track data.<br/>
    Just add <tt>/json</tt> to most urls to get a JSON representation of that page.<br/>
    JSONP is a convenience for Javascript users. Adding a <tt>jsonp</tt> parameter will insert the JSON data as an argument to a function named by the <tt>jsonp</tt> parameter. It can then be run by loading it in a &lt;script&gt; tag.
    <small><a href="https://en.wikipedia.org/wiki/JSONP">More info on JSONP.</a></small><br/>
    Compare <a href="http://eqbeats.org/tracks/latest/json">/tracks/latest/json</a> and <a href="http://eqbeats.org/tracks/latest/json?jsonp=myCallback">/tracks/latest/json?jsonp=myCallback</a>.<br/>
    The API is also CORS-enabled: it can be accessed with a plain old XMLHttpRequest from any domain.</p>
    <p>There are no hard-limits to API usage, but if you intend to poll it continuously, please consider setting a reasonable delay like 3 seconds.</p>

    <h3 id="user">User object</h3>
    <table>
        <tr><td><tt>id</tt></td><td>Unique numerical id for this user.</td></tr>
        <tr><td><tt>name</tt></td><td>User name.</td></tr>
        <tr><td><tt>avatar</tt></td><td>URL of the user's avatar.</td></tr>
        <tr><td><tt>description</tt></td><td>User's description of themselves, raw. This member will not exist if there is no description.</td></tr>
        <tr><td><tt>html_description</tt></td><td>Same as above, formatted to HTML.</td></tr>
        <tr><td><tt>tracks</tt></td><td>An array of <a href="#track">Track objects</a> for all tracks published by the user.</td></tr>
        <tr><td><tt>playlists</tt></td><td>Array of <a href="#playlist">Playlist objects</a> (id, name, author and link only).</td></tr>
        <tr><td><tt>link</tt></td><td>Permalink to the user's page.</td></tr>
    </table>
    <pre>{
    "id": 123,
    "name": "Foo Bar",
    "avatar": "http://gravatar.com/000011112222333344445555deadbeef",
    "description": "Just some dude making [b]music[/b].",
    "html_description": "Just some dude making &lt;b&gt;music&lt;/b&gt;.",
    "tracks": [{"id": 456, ...}, ...],
    "playlists": [{"id": 789, ...}, ...],
    "link": "http://eqbeats.org/user/123"
}</pre>

    <h3 id="track">Track object</h3>
    <table>
        <tr><td><tt>id</tt></td><td>Unique numerical id for the track.</td></tr>
        <tr><td><tt>title</tt></td><td>Track title.</td></tr>
        <tr><td><tt>description</tt></td><td>Track notes, raw. This member will not exist if there are no notes.</td></tr>
        <tr><td><tt>html_description</tt></td><td>Same as above, formatted to HTML.</td></tr>
        <tr><td><tt>artist</tt></td><td>A <a href="#user">user object</a> for the artist (excluding the track array).</td></tr>
        <tr><td><tt>link</tt></td><td>Permalink to the track's page.</td></tr>
        <tr><td><tt>download</tt></td><td>Permalinks to the downloadable / streamable audio files. It will also contain an <tt>art</tt> value pointing to the image if the track has cover art.</td></tr>
    </table>
    <pre>{
    "id": 456,
    "title": "Baz Quux",
    "description": "A track about bazing while you quux.",
    "html_description": "A track about bazing while you quux.",
    "artist": {"id": 123, "name": "Foo Bar",...},
    "link": "http://eqbeats.org/track/456",
    "download": {"art":"http://eqbeats.org/track/456/art",
                 "opus":"http://eqbeats.org/track/456/opus",
                 "vorbis":"http://eqbeats.org/track/456/vorbis",
                 "aac":"http://eqbeats.org/track/456/aac",
                 "mp3":"http://eqbeats.org/track/456/mp3",
                 "original":"http://eqbeats.org/track/456/original"
                 }
}</pre>

    <h3 id="playlist">Playlist object</h3>
    <table>
        <tr><td><tt>id</tt></td><td>Unique numerical id for the playlist.</td></tr>
        <tr><td><tt>name</tt></td><td>Playlist name.</td></tr>
        <tr><td><tt>description</tt></td><td>Description, raw. This member will not exist if there is no description.</td></tr>
        <tr><td><tt>html_description</tt></td><td>Same as above, formatted to HTML.</td></tr>
        <tr><td><tt>author</tt></td><td>A <a href="#user">user object</a> for the author.</td></tr>
        <tr><td><tt>tracks</tt></td><td>An array of <a href="#track">Track objects</a>.</td></tr>
        <tr><td><tt>link</tt></td><td>Permalink to the playlist's page.</td></tr>
    </table>

    <h3>Resources</h3>
    <table>
        <tr><td><tt>/user/<var>{id}</var>/json</tt></td><td>One <a href="#user">user object</a>.</td></tr>
        <tr><td><tt>/users/search/json?q=<var>{query}</var></tt></td><td>[Array] <a href="#user">User objects</a> matching the query.</td></tr>
        <tr><td><tt>/user/<var>{id}</var>/favorites/json</tt></td><td>[Array] Favorite tracks.</td></tr>
        <tr><td><tt>/artists/json</tt></td><td>[Array] All artists (as shortened <a href="#user">user objects</a>, without their tracks).</td></tr>
        <tr><td><tt>/track/<var>{id}</var>/json</tt></td><td>One <a href="#track">track object</a>.</td></tr>
        <tr><td><tt>/tracks/search/json?q=<var>{query}</var></tt></td><td>[Array] <a href="#track">Track objects</a> matching the query.</td></tr>
        <tr><td><tt>/tracks/search/exact/json?artist=<var>{artist}</var>&amp;track=<var>{track}</var></tt></td><td>[Array] <a href="#track">Track object</a> matching the artist and track name exactly (case sensitive).</td></tr>
        <tr><td><tt>/tracks/latest/json</tt></td><td>[Array] The latest 50 tracks.</td></tr>
        <tr><td><tt>/tracks/featured/json</tt></td><td>[Array] The latest 50 featured tracks.</td></tr>
        <tr><td><tt>/tracks/random/json</tt></td><td>[Array] 50 random tracks.</td></tr>
        <tr><td><tt>/tracks/all/json</tt></td><td>[Array] Every published track.</td></tr>
        <tr><td><tt>/playlist/<var>{id}</var>/json</tt></td><td>One <a href="#playlist">playlist object</a>.</td></tr>
    </table>
</div>
