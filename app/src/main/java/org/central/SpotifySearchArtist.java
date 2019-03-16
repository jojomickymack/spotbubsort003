package org.central;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class SpotifySearchArtist implements Runnable {
    private String response = "no response received";
    private String searchEndpoint = "https://api.spotify.com/v1/search";
    private String searchTerm = "";
    private String token = "";

    public SpotifySearchArtist (String token) {
        this.token = token;
    }

    public void setSearchTerm(String searchTerm) {
        this.searchTerm = searchTerm;
    }

    @Override
    public void run() {
        if (token == "") {
            this.response = "no token";
        } else {
            try {
                if (searchTerm == "") searchTerm = "thin%20lizzy";
                URL url = new URL(searchEndpoint + "?q=" + searchTerm + "&type=artist");
                HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                conn.setRequestProperty("Authorization", "Bearer " + this.token);
                try {
                    InputStream is = new BufferedInputStream(conn.getInputStream());
                    int ch;
                    StringBuilder sb = new StringBuilder();
                    while ((ch = is.read()) != -1) {
                        sb.append((char) ch);
                    }
                    this.response = sb.toString();
                } finally {
                    conn.disconnect();
                }
            } catch (IOException e) {
                System.out.println("there was an error " + e);
            }
        }
    }

    public String getResponse() {
        return this.response;
    }
}

