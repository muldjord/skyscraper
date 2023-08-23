FROM debian:bookworm-slim as build
COPY . /skyscraper
RUN apt update \
    && apt install -y build-essential qtbase5-dev \
    && cd skyscraper && qmake && make

FROM scratch as export
COPY --from=build /skyscraper/Skyscraper /skyscraper/artwork.xml /
