# RSA

This is my implementation of the [RSA
Algorithm](https://en.wikipedia.org/wiki/RSA_(cryptosystem)).  There are three
components, `rsa-keygen`, `rsa-encode` and `rsa-decode`. These components are
explained below. The project was inspired from the book [From Mathematics to
Generic Programming](https://www.fm2gp.com) by Alexander Stepanov and Dan Rose.
This implementation is only for educative purposes.

### Key Generation

The `rsa-keygen` executable will generate two keys, a public and a private one,
that will be used later by the encoder and decoder respectively. The keys are
stored in a text file as [base64](https://en.wikipedia.org/wiki/Base64)
encoding.

```sh
Usage: rsa-keygen <public_key> <private_key>
```

The `<public_key>` is the name of the file that will hold the public key that
is generated. This will be used later by `rsa-encode` to encode messages. The
public key can be given to others who can then encode messages.

The `<private_key>` is the name of the file that will hold the private key that
is generated. This will be used later by `rsa-decode` to decode an encoded
message (generated by `rsa-encode`). The private key is not meant to be shared.

### Encoding

The `rsa-encode` executable will encode messages using a public key. These
messages can only be decoded by using `rsa-decode` with the associated private
key.

```sh
Usage: rsa-encode <public_key> <input> <output>
```

The `<public_key>` is the name of the file that contains the key generated by
`rsa-keygen`. This allows a user to publicly specify their public key allowing
others to encode messages that can only be decoded using `rsa-decode`.

The `<input>` is the name of the file that contains the contents that are to be
encoded.

The `<output>` is the name of the file that will contain the encoded message.

### Decoding

The `rsa-decode` executable takes your private key and an encoded message and
outputs the decoded message to a file.

```sh
Usage: rsa-decode <private_key> <input> <output>
```

The `<private_key>` is the name of the file that contains your private key
generated by `rsa-keygen`.

The `<input>` is the name of the file that contains the encoded message. This
file should've been generated by `rsa-encode`.

The `<output>` is the name of the file that will store the decoded message.
